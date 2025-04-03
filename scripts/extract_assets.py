import re
import os
import json
import shutil

fonts = []
images = []
fontTypes = {
    "A": "ALPHABETS",
    "N": "NUMBERS",
    "AN": "ALPHA_NUMERIC",
    "F": "ONLY_F"
}

with open('../editor_files/assets/assets.ese', 'r') as file:
    assets = json.load(file)
    contents = assets['content']
    for content in contents:
        if content['converter'] == 'Font':
            name = content['displayName']
            name = name.split(".")[0].upper()
            fontType = fontTypes[name.split("_")[2]]
            name = "_".join(name.split("_")[:3])
            fileName = content['destName']+'.raw'
            size = os.path.getsize(f'../editor_files/assets/{fileName}')
            
            fonts.append({'name': name, 
                          'file': fileName, 
                          'type': fontType,
                          'memoryAddress': content['memoryAddress'],
                          'startChar': content['fontOffset'],
                          'size': size})
        elif content['converter'] == 'Image':
            name = content['displayName']
            name = name.split(".")[0].upper()
            # name = "_".join(name.split("_")[:4])
            fileName = content['destName']+'.raw'
            size = os.path.getsize(f'../editor_files/assets/{fileName}')
            image = {'name': name, 
                    'file': fileName,
                    'memoryAddress': content['memoryAddress'],
                    'size': size}
            # print contents of file
            with open(f'../editor_files/assets/{content['destName']+'.json'}', 'r') as file:
                meta = json.load(file)
                image['width'] = meta['width']
                image['height'] = meta['height']
                image['format'] = meta['format']
                print(image['format'])
            images.append(image)

# Copy the fonts and images to the data folder for littleFS
if not os.path.exists('../eve_arduino/data/fonts'):
    os.makedirs('../eve_arduino/data/fonts')
if not os.path.exists('../eve_arduino/data/images'):
    os.makedirs('../eve_arduino/data/images')

for font in fonts:
    shutil.copy(f'../editor_files/assets/{font["file"]}', '../eve_arduino/data/fonts/')
for image in images:
    shutil.copy(f'../editor_files/assets/{image["file"]}', '../eve_arduino/data/images/')

# Generate the assets.h file

if not os.path.exists('../eve_arduino/assets'):
    os.makedirs('../eve_arduino/assets')
with open('../eve_arduino/assets/info.h', 'w') as file:
    file.write("#ifndef ASSETS_INFO_H\n")
    file.write("#define ASSETS_INFO_H\n")
    file.write("#include \"asset.h\"\n")
    file.write('enum Fonts {\n')
    for font in fonts:
        file.write(f'    FONT_{font["name"]},\n')
    file.write('};\n\n')
    file.write('enum Images {\n')
    for image in images:
        file.write(f'    IMAGE_{image["name"]},\n')
    file.write('};\n\n')

    file.write('void load_assets();\n')
    file.write('#endif\n')

# Generate the assets.cpp file
with open('../eve_arduino/assets/info.cpp', 'w') as file:
    file.write("#include \"info.h\"\n")
    file.write('const Font fonts[] = {\n')
    for font in fonts:
        file.write(f'    {{{font["memoryAddress"]}, {font["size"]}, FONT_TYPE_{font["type"]}, {font["startChar"]}}}, // {font["name"]}\n')
    file.write('};\n\n')

    file.write('const Image images[] = {\n')
    for image in images:
        file.write(f'    {{{image["memoryAddress"]}, {image["size"]}, {image["width"]}, {image["height"]}, IMAGE_FORMAT_{image["format"]}}}, // {image["name"]}\n')
    file.write('};\n\n')
    file.write('\n\nvoid load_assets() {\n')
    # for each font open the file using LittleFS and load it using loadImageFromLittleFS(const char* filename, uint32_t ram_g_addr)
    for font in fonts:
        file.write(f'    load_asset_from_littlefs("/{font["file"]}", {font["memoryAddress"]});\n')
    for image in images:
        file.write(f'    load_asset_from_littlefs("/{image["file"]}", {image["memoryAddress"]});\n')
    file.write('}\n')
