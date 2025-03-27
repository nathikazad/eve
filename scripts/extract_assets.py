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

with open('../editor_files/assets.ese', 'r') as file:
    assets = json.load(file)
    contents = assets['content']
    for content in contents:
        if content['converter'] == 'Font':
            name = content['displayName']
            name = name.split(".")[0].upper()
            name = "_".join(name.split("_")[:4])
            fileName = content['destName']+'.raw'
            size = os.path.getsize(f'../editor_files/{fileName}')
            fontType = fontTypes[name.split("_")[2]]
            fonts.append({'name': name, 
                          'file': fileName, 
                          'type': fontType,
                          'memoryAddress': content['memoryAddress'],
                          'size': size})
        elif content['converter'] == 'Image':
            name = content['displayName']
            name = name.split(".")[0].upper()
            name = "_".join(name.split("_")[:4])
            fileName = content['destName']+'.raw'
            size = os.path.getsize(f'../editor_files/{fileName}')
            image = {'name': name, 
                    'file': fileName,
                    'memoryAddress': content['memoryAddress'],
                    'size': size}
            # print contents of file
            with open(f'../editor_files/{content['destName']+'.json'}', 'r') as file:
                meta = json.load(file)
                image['width'] = meta['width']
                image['height'] = meta['height']
                image['format'] = meta['format']
                print(image['format'])
            images.append(image)

for font in fonts:
    shutil.copy(f'../editor_files/{font["file"]}', '../eve_arduino/data/')
for image in images:
    shutil.copy(f'../editor_files/{image["file"]}', '../eve_arduino/data/')

if not os.path.exists('../eve_arduino/gen'):
    os.makedirs('../eve_arduino/gen')
with open('../eve_arduino/assets.h', 'w') as file:
    with open('../scripts/asset_template.h', 'r') as header:
        file.write(header.read())
    file.write('enum Fonts {\n')
    for font in fonts:
        file.write(f'    FONT_{font["name"]},\n')
    file.write('};\n\n')
    file.write('enum Images {\n')
    for image in images:
        file.write(f'    IMAGE_{image["name"]},\n')
    file.write('};\n\n')

    file.write('const Font fonts[] = {\n')
    for font in fonts:
        file.write(f'    {{{font["memoryAddress"]}, {font["size"]}, FONT_TYPE_{font["type"]}}},\n')
    file.write('};\n\n')

    file.write('const Image images[] = {\n')
    for image in images:
        file.write(f'    {{"{image["name"]}", {image["memoryAddress"]}, {image["size"]}, {image["width"]}, {image["height"]}, IMAGE_FORMAT_{image["format"]}}},\n')
    file.write('};\n')

with open('../eve_arduino/assets.cpp', 'w') as file:
    with open('../scripts/asset_template.cpp', 'r') as template:
        file.write(template.read())
    file.write('\n\nvoid loadAssetsIntoRAM() {\n')
    # for each font open the file using LittleFS and load it using loadImageFromLittleFS(const char* filename, uint32_t ram_g_addr)
    for font in fonts:
        file.write(f'    loadAssetFromLittleFS("{font["file"]}", {font["memoryAddress"]});\n')
    for image in images:
        file.write(f'    loadAssetFromLittleFS("{image["file"]}", {image["memoryAddress"]});\n')
    file.write('}\n')
