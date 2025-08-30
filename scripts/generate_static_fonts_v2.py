#!/usr/bin/env python3
"""
Script to generate static TTF files from Nunito Sans variable fonts.
Generates one TTF file for each named instance using a simpler approach.
"""

import os
import shutil
from fontTools.ttLib import TTFont
from fontTools.varLib.instancer import instantiateVariableFont

def get_instance_name(font, name_id):
    """Get the actual name string from a name ID."""
    try:
        name_table = font['name']
        for name_record in name_table.names:
            if name_record.nameID == name_id:
                return name_record.toUnicode()
    except:
        pass
    return f"Instance_{name_id}"

def generate_static_fonts(font_path, output_dir):
    """Generate static TTF files for each named instance."""
    print(f"\nProcessing: {os.path.basename(font_path)}")
    print("=" * 50)
    
    try:
        # Load the variable font
        font = TTFont(font_path)
        
        # Check if it's a variable font
        if 'fvar' not in font:
            print("This is not a variable font!")
            return
        
        # Get the fvar table (font variations)
        fvar = font['fvar']
        
        # Create output directory if it doesn't exist
        os.makedirs(output_dir, exist_ok=True)
        
        # Get base font name for output files
        base_name = os.path.splitext(os.path.basename(font_path))[0]
        base_name = base_name.replace('-VariableFont_YTLC,opsz,wdth,wght', '')
        
        print(f"Found {len(fvar.instances)} named instances:")
        
        # Generate static fonts for each named instance
        for i, instance in enumerate(fvar.instances, 1):
            # Get the instance name
            instance_name = get_instance_name(font, instance.subfamilyNameID)
            print(f"  {i}. {instance_name}")
            
            # Create output filename
            safe_name = instance_name.replace(' ', '-').replace('/', '-').replace('\\', '-')
            output_filename = f"{base_name}-{safe_name}.ttf"
            output_path = os.path.join(output_dir, output_filename)
            
            # Create a new font instance with the specific coordinates
            coordinates = dict(instance.coordinates)
            
            # Use instantiateVariableFont to create the static version
            static_font = instantiateVariableFont(font, coordinates)
            
            # Save the static font
            static_font.save(output_path)
            static_font.close()
            
            print(f"     → Saved: {output_filename}")
        
        font.close()
        print(f"\nGenerated {len(fvar.instances)} static TTF files in: {output_dir}")
        
    except Exception as e:
        print(f"Error processing font: {e}")
        import traceback
        traceback.print_exc()

def main():
    """Main function to generate static fonts from both variable font files."""
    print("Nunito Sans Static Font Generator v2")
    print("=" * 50)
    
    # Paths to the variable font files
    font_dir = "../Nunito_Sans"
    regular_font = os.path.join(font_dir, "NunitoSans-VariableFont_YTLC,opsz,wdth,wght.ttf")
    italic_font = os.path.join(font_dir, "NunitoSans-Italic-VariableFont_YTLC,opsz,wdth,wght.ttf")
    
    # Output directory for generated fonts
    output_dir = "../Nunito_Sans/static_fonts"
    
    # Process regular font
    if os.path.exists(regular_font):
        generate_static_fonts(regular_font, output_dir)
    else:
        print(f"Regular font not found: {regular_font}")
    
    # Process italic font
    if os.path.exists(italic_font):
        generate_static_fonts(italic_font, output_dir)
    else:
        print(f"Italic font not found: {italic_font}")
    
    print(f"\nAll static fonts have been generated in: {output_dir}")

if __name__ == "__main__":
    main()

