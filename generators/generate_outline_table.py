def make_pattern_int(s: str):
    return int(s.replace("\n", "").replace(" ", "").replace(".", "0").replace("#", "1"), 2)

def make_pattern_string(p: int):
    "Turn a pattern back into a string for debugging purposes."
    points = list(bin(p)[2:].zfill(8).replace("0", ".").replace("1", "#"))
    return "{} {} {}\n{}   {}\n{} {} {}".format(*points)

MAP = [
    None,
    make_pattern_int(
        """
            . . .
            .   .
            . . .
        """
    ),
    make_pattern_int(
        """
            . . .
            .   .
            . . #
        """
    ),
    make_pattern_int(
        """
            . . .
            .   .
            # . #
        """
    ),
    make_pattern_int(
        """
            # . .
            .   .
            . . #
        """
    ),
    make_pattern_int(
        """
            . . #
            .   .
            . . #
        """
    ),
    make_pattern_int(
        """
            . . #
            .   .
            # . #
        """
    ),
    make_pattern_int(
        """
            # . #
            .   .
            # . #
        """
    ),
    make_pattern_int(
        """
            . . .
            .   .
            # # #
        """
    ),
    make_pattern_int(
        """
            . . #
            .   .
            # # #
        """
    ),
    make_pattern_int(
        """
            # . #
            .   .
            # # #
        """
    ),
    make_pattern_int(
        """
            . . #
            .   #
            . . #
        """
    ),
    make_pattern_int(
        """
            . . #
            .   #
            # . #
        """
    ),
    make_pattern_int(
        """
            # . #
            .   #
            # . #
        """
    ),
    make_pattern_int(
        """
            . . #
            .   #
            # # #
        """
    ),
    make_pattern_int(
        """
            # . #
            .   #
            # # #
        """
    ),
    make_pattern_int(
        """
            # # #
            .   .
            # # #
        """
    ),
    make_pattern_int(
        """
            # . #
            #   #
            # . #
        """
    ),
    make_pattern_int(
        """
            # # #
            .   #
            # # #
        """
    ),
    make_pattern_int(
        """
            # # #
            #   #
            # . #
        """
    ),
    make_pattern_int(
        """
            # # #
            #   #
            # # #
        """
    ),
]

ROTATE_MAP = [
    5, 3, 0,
    6,    1,
    7, 4, 2,
]

HFLIP_MAP = [
    2, 1, 0,
    4,    3,
    7, 6, 5,
]

VFLIP_MAP = [
    5, 6, 7,
    3,    4,
    0, 1, 2,
]

def apply_bit_map(p: int, m: list[int]):
    bin_representation = bin(p)[2:].zfill(8)
    return int("".join(bin_representation[i] for i in m), 2)

TOP_EDGE_POINT = make_pattern_int(
    """
        . # .
        .   .
        . . .
    """
)
TOP_EDGE_FULL = make_pattern_int(
    """
        # # #
        .   .
        . . .
    """
)

EDGE_POINTS = [
    TOP_EDGE_POINT,
]
FULL_EDGES = [
    TOP_EDGE_FULL,
]
for _ in range(3):
    EDGE_POINTS.append(apply_bit_map(EDGE_POINTS[-1], ROTATE_MAP))
    FULL_EDGES.append(apply_bit_map(FULL_EDGES[-1], ROTATE_MAP))

def clean_up_pattern(p: int):
    for i in range(4):
        if p & EDGE_POINTS[i]:
            p |= FULL_EDGES[i]
    return p

FLIP_HORIZONTAL_BIT = 0b0100_0000_0000_0000
FLIP_VERTICAL_BIT = 0b1000_0000_0000_0000

table = []
for i in range(256):
    pattern = clean_up_pattern(i)
    if pattern in MAP:
        table.append(MAP.index(pattern))
        continue
    hflip_pattern = apply_bit_map(pattern, HFLIP_MAP)
    if hflip_pattern in MAP:
        table.append(MAP.index(hflip_pattern) | FLIP_HORIZONTAL_BIT)
        continue
    hvflip_pattern = apply_bit_map(hflip_pattern, VFLIP_MAP)
    if hvflip_pattern in MAP:
        table.append(MAP.index(hvflip_pattern) | FLIP_HORIZONTAL_BIT | FLIP_VERTICAL_BIT)
        continue
    vflip_pattern = apply_bit_map(pattern, VFLIP_MAP)
    if vflip_pattern in MAP:
        table.append(MAP.index(vflip_pattern) | FLIP_VERTICAL_BIT)
        continue
    assert False, "Pattern not found in map"

print(table)

table_data = b''.join(pattern.to_bytes(2, "little") for pattern in table)

with open("data/generated/outline_table.bin", "wb") as file:
    file.write(table_data)
