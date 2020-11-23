import json

filenames = [
"000000_bottom_horizontal_border.png",
"010110_top_left_border.png",
"000001_bottom_right_border.png",
"101101_right_border.png",
"001101_top_right_border.png",
"101110_horizontal_border.png",
"001110_top_horizontal_border.png",
"110101_central.png",
"010000_bottom_left_border.png",
"110110_left_border.png",
"010001_bottom_border.png",
"010101_top_border.png"
]

walls = {}

file = [filename for filename in filenames if "bottom" not in filename]

for x in range(64):
    entry = {}
    value = bin(x)[2:].zfill(6)
    upper_file = [filename for filename in filenames if "bottom" not in filename and filename.split("_")[0][0] == value[0] and filename.split("_")[0][1] == value[1] and filename.split("_")[0][5] == value[5]]
    lower_file = [filename for filename in filenames if "top" not in filename and filename.split("_")[0][1] == value[1] and (filename.split("_")[0][2] or value[2]) and filename.split("_")[0][3] == value[3] and (filename.split("_")[0][4] or value[4]) and filename.split("_")[0][5] == value[5]]
    print(upper_file)
    print(lower_file)
    entry["upper"] = upper_file[0]
    entry["lower"] = lower_file[0]
    walls[x] = entry;
print("\n")
print(json.dumps(walls, indent=4))
