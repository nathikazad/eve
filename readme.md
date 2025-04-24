# Structure

### 1. Editor Files
This folder has all the eve co processor commands for the different screens inside editor_files/FlowChart.pdf It also has the editor_files/assets/assets.ese file which is used to build the assets(fonts and images) to import into the file partition of esp32. Developer will need [Eve Screen Editor](https://brtchip.com/ese-2/) to open the .ese file, refer to this [guide]https://brtchip.com/wp-content/uploads/2025/03/BRT_AN_037_EVE-Screen-Editor-5.0-User-Guide.pdf) to learn how to use Eve Screen Editor.

#### Understanding the way bridgetek works
The way building screens for eve works like this, there is special bridgetek chip which is connected to a lcd touch screen. A main processor(in this case esp32-s3) is connected to bridgetek chip via SPI and it sends the bridgetek chip special commands, called **co-processor commands** to display texts, shapes and images. The chip also sends interrupts when it registers a touch event and can be asked for the touch coordinates. 
The EVE screen editor is essentially a simulator where a developer can try out the different **co-processor commands** and see the resulting output of the lcd screen. Inside the editor the size of lcd and type of chip being used can be selected.

### 2. Eve Arduino
This folder has a sample arduino project that displays the screens from editor files on esp32. It is mainly for testing purpose.

### 3. Resources
This folder has all the fonts in their original format and generated versions for use in esp32. There are generated version because the bt chips are not capable of resizing fonts, instead fonts have to be generated for the different sizes which they will be used for and then exported into RAM of the bridgetek chip, so it will be used appropriately. To save space, not all characters are generated, sometimes it is only numbers, only characters or both.

The file name convention for generated fonts is
[weight]_[size]_[chosen_characters]_[ascii_of_first_character].otf

Look inside scripts/font_extractor.py to understand how to generate a font.

### 4. Scripts
Scripts are used to help with generating fonts of different sizes and then to move the built assets from editor files into file storage partition of arduino.

## Putting it all together
It is a multistep process to build a screen. First, developer must see all the font sizes necessary for a particular screen and minimum characters necessary for each font size. Then using the script font_extractor.py, he must generate the font size and character necessary. For example, to generate font size 12, weight 65, only numbers,he will execute `python font_extractor.py 65 12 numeric'. The generated font will be inside resources/fonts_generated. Then using the eve screen editor the generated font has to be imported into RAM.