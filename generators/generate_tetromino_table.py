type Point = tuple[int, int]

PIECES = [
    # I piece
    [
        (-1, 0),
        (0, 0),
        (1, 0),
        (2, 0),
    ],
    # T piece
    [
        (-1, 0),
        (0, 0),
        (0, -1),
        (1, 0),
    ],
    # S piece
    [
        (-1, 0),
        (0, 0),
        (0, -1),
        (1, -1),
    ],
    # J piece
    [
        (-1, -1),
        (-1, 0),
        (0, 0),
        (1, 0),
    ],
    # O piece
    [
        (0, -1),
        (1, -1),
        (0, 0),
        (1, 0),
    ],
    # Z piece
    [
        (-1, -1),
        (0, 0),
        (0, -1),
        (1, 0),
    ],
    # L piece
    [
        (-1, 0),
        (0, 0),
        (1, 0),
        (1, -1),
    ],
]

def rotate_point(point: Point, times: int = 1) -> Point:
    if times == 0:
        return point
    if times > 1:
        point = rotate_point(point, times - 1)
    return (-point[1], point[0])

serialized_x_positions: list[bytes] = []
serialized_y_positions: list[bytes] = []
for piece in PIECES:
    for i in range(4):
        for point in piece:
            rotated_point = rotate_point(point, i)
            serialized_x_positions.append(rotated_point[0].to_bytes(1, signed=True))
            serialized_y_positions.append(rotated_point[1].to_bytes(1, signed=True))
serialized_x_positions = b''.join(serialized_x_positions)
serialized_y_positions = b''.join(serialized_y_positions)

with open("data/generated/tetromino_table_x.bin", "wb") as file:
    file.write(serialized_x_positions)

with open("data/generated/tetromino_table_y.bin", "wb") as file:
    file.write(serialized_y_positions)
