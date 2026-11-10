#!/usr/bin/env python3
import os
import shutil
import subprocess
import sys
from pathlib import Path

VERSION = "v2026.11.3"
CLANG = "clang"
ROOT = Path(__file__).parent.resolve()
SRC_DIR = ROOT / "src"
DIST_DIR = ROOT / "dist"
BUILD_ROOT = ROOT / "build"
LINKER_SRC = ROOT / "linker.ld"

BASE_SRCS = [
    "src/startup.c",
    "src/board.c",
    "src/display.c",
    "src/font.c",
    "src/dmm.c",
    "src/settings.c",
    "src/fpga.c",
    "src/scope.c",
    "src/siggen.c",
    "src/fw_update.c",
    "src/screenshot.c",
    "src/w25q.c",
    "src/usb_msc.c",
    "src/bootloader.c",
    "src/ui.c",
    "src/main.c",
    "src/fft.c",
    "src/arb_csv.c",
]

TARGETS = [
    {
        "name": "lt-hw4",
        "app_base": "0x08008000",
        "app_flash_size": "0x00038000",
        "fw_stage_base": "0x08040000",
        "hw40": "0",
        "bitstream": "src/fpga_bitstream.c",
        "output_bin": f"F2C23T-{VERSION}-08008000.bin",
    },
    {
        "name": "hw4.0",
        "app_base": "0x08007000",
        "app_flash_size": "0x00045000",
        "fw_stage_base": "0x08050000",
        "hw40": "1",
        "bitstream": "src/fpga_bitstream_hw4.c",
        "output_bin": f"F2C23T-{VERSION}-HW4.0-08007000.bin",
    },
]


def run_cmd(cmd, cwd=ROOT):
    proc = subprocess.run(cmd, cwd=cwd)
    if proc.returncode != 0:
        raise RuntimeError(f"Command failed with code {proc.returncode}: {' '.join(str(c) for c in cmd)}")


def build_target(cfg):
    target_name = cfg["name"]
    print(f"=== Building target: {target_name} ===")
    build_dir = BUILD_ROOT / target_name
    build_dir.mkdir(parents=True, exist_ok=True)

    app_base = cfg["app_base"]
    app_flash_size = cfg["app_flash_size"]
    fw_stage_base = cfg["fw_stage_base"]
    hw40 = cfg["hw40"]

    # 1. Prepare linker script
    linker_script = build_dir / "linker.ld"
    linker_text = LINKER_SRC.read_text(encoding="utf-8")
    linker_text = linker_text.replace("ORIGIN = 0x08008000", f"ORIGIN = {app_base}")
    linker_text = linker_text.replace("LENGTH = 224K", f"LENGTH = {app_flash_size}")
    linker_script.write_text(linker_text, encoding="utf-8")

    # 2. Compile objects
    cflags = [
        "-target", "armv7em-none-eabi",
        "-mcpu=cortex-m4",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=softfp",
        "-mthumb",
        "-ffreestanding",
        "-fno-builtin",
        "-fdata-sections",
        "-ffunction-sections",
        "-fno-unwind-tables",
        "-fno-asynchronous-unwind-tables",
        "-Oz",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-Wno-unused-but-set-variable",
        "-Wno-unused-but-set-global",
        "-I", str(SRC_DIR),
        f"-DAPP_BASE_ADDR={app_base}u",
        f"-DAPP_FLASH_SIZE_BYTES={app_flash_size}u",
        f"-DFW_STAGE_BASE_ADDR={fw_stage_base}u",
        f"-DHW_TARGET_HW40={hw40}",
        "-DDMM_UART_BAUD=9600",
        "-DFPGA_SPI_BR=2",
        "-DSCOPE_HW_CAPTURE=1",
        "-DSCOPE_UI_SAFE_STUB=0",
        "-DSCOPE_ANALOG_CONFIG=1",
        "-DSCOPE_ATTENUATOR_CONFIG=1",
    ]

    srcs = BASE_SRCS + [cfg["bitstream"]]
    objs = []
    for src in srcs:
        src_path = ROOT / src
        obj_path = build_dir / (src_path.stem + ".o")
        objs.append(obj_path)
        print(f"  CC {src}")
        run_cmd([CLANG] + cflags + ["-c", str(src_path), "-o", str(obj_path)])

    # 3. Link ELF
    elf_path = build_dir / "f2c23t_hello.elf"
    map_path = build_dir / "f2c23t_hello.map"
    ldflags = [
        "-target", "armv7em-none-eabi",
        "-mcpu=cortex-m4",
        "-mfpu=fpv4-sp-d16",
        "-mfloat-abi=softfp",
        "-mthumb",
        "-nostdlib",
        "-fuse-ld=lld",
        f"-Wl,-T,{linker_script}",
        "-Wl,--gc-sections",
        f"-Wl,-Map,{map_path}",
    ]
    print(f"  LD {elf_path.name}")
    run_cmd([CLANG] + ldflags + [str(o) for o in objs] + ["-o", str(elf_path)])

    # 4. Convert to BIN
    bin_path = build_dir / "f2c23t_hello.bin"
    print(f"  BIN {bin_path.name}")
    run_cmd([sys.executable, str(ROOT / "tools" / "elf2bin.py"), str(elf_path), str(bin_path), app_base])

    # 5. Copy to dist
    DIST_DIR.mkdir(parents=True, exist_ok=True)
    out_bin = DIST_DIR / cfg["output_bin"]
    shutil.copy2(bin_path, out_bin)
    print(f"  -> Generated {out_bin} ({out_bin.stat().st_size:,} bytes)\n")


def main():
    if DIST_DIR.exists():
        shutil.rmtree(DIST_DIR)
    for target in TARGETS:
        build_target(target)
    print("All builds completed successfully!")


if __name__ == "__main__":
    main()
