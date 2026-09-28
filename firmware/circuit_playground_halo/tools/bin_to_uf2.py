#!/usr/bin/env python3
"""Wrap a Circuit Playground Express application binary in UF2 blocks."""
from pathlib import Path
import argparse
import math
import struct

MAGIC_START_0 = 0x0A324655
MAGIC_START_1 = 0x9E5D5157
MAGIC_END = 0x0AB16F30
FLAG_FAMILY_ID_PRESENT = 0x00002000
SAMD21_FAMILY_ID = 0x68ED2B88
APPLICATION_START = 0x2000
PAYLOAD_SIZE = 256


def convert(source: Path, destination: Path) -> None:
    data = source.read_bytes()
    block_count = math.ceil(len(data) / PAYLOAD_SIZE)
    with destination.open("wb") as output:
        for block_number in range(block_count):
            payload = data[
                block_number * PAYLOAD_SIZE:(block_number + 1) * PAYLOAD_SIZE
            ].ljust(PAYLOAD_SIZE, b"\x00")
            header = struct.pack(
                "<8I",
                MAGIC_START_0,
                MAGIC_START_1,
                FLAG_FAMILY_ID_PRESENT,
                APPLICATION_START + block_number * PAYLOAD_SIZE,
                PAYLOAD_SIZE,
                block_number,
                block_count,
                SAMD21_FAMILY_ID,
            )
            output.write(header + payload + bytes(220) + struct.pack("<I", MAGIC_END))
    assert destination.stat().st_size == block_count * 512


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    convert(args.source, args.destination)
