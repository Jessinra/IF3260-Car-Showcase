in_folder_prefix = "C:\Code\C++\grafkomGL\Asset\Dragon\cleaned\\"
out_folder_prefix = "C:\Code\C++\grafkomGL\Asset\Dragon\scripted\\"


def generate_formatted_file(asset):
    input_file = open(in_folder_prefix + asset, mode="r", encoding="utf-8")
    output_file = open(out_folder_prefix + asset, mode="w")

    input_data = [x.strip().split() for x in input_file.readlines()]

    window_width = int(input_data[0][0])
    window_height = int(input_data[0][1])
    vertex_counter_offset = int(input_data[0][2])
    vertex_counter = 0

    out_line = "{:0>3d} {:0>3d}\n".format(window_width, window_height)
    output_file.write(out_line)

    for i in range(1, len(input_data)):

        current_line = input_data[i]

        # section title
        if len(current_line) == 1:

            section_title = current_line[0]
            out_line = "{}\n".format(section_title)

        # Vertices
        elif len(current_line) == 2:

            vertex_x = int(current_line[0]) / window_width
            vertex_y = int(current_line[1]) / window_height
            vertex_z = 0.0

            vertex_id = vertex_counter_offset + vertex_counter
            vertex_counter += 1

            out_line = "{:0>3d} {:.3f} {:.3f} {:.3f}\n".format(vertex_id, vertex_x, vertex_y, vertex_z)

        # Indices for creating triangles
        elif len(current_line) == 3:

            ind_01 = int(current_line[0]) + vertex_counter_offset
            ind_02 = int(current_line[1]) + vertex_counter_offset
            ind_03 = int(current_line[2]) + vertex_counter_offset

            out_line = "{:0>3d} {:0>3d} {:0>3d}\n".format(ind_01, ind_02, ind_03)

        # Color setting
        elif len(current_line) == 4 and current_line[0].lower() == "color":

            color_r = int(current_line[1]) / 255
            color_g = int(current_line[2]) / 255
            color_b = int(current_line[3]) / 255

            out_line = "Color {:.3f} {:.3f} {:.3f}\n".format(color_r, color_g, color_b)

        else:
            out_line = current_line

        output_file.write(out_line)
        # print()


if __name__ == "__main__":

    asset_filename = [
        "dragon_body.txt",
        "dragon_chest.txt",
        "dragon_head.txt",
        "dragon_horn.txt",
        "dragon_limb.txt",
        "dragon_tail.txt",
        "dragon_wing.txt",
    ]

    for asset in asset_filename:
        generate_formatted_file(asset)
