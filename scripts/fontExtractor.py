#!/usr/bin/env python3
"""
Font Subsetter - Extract characters from an OTF font file based on preset ranges.
Usage: python font_subsetter.py [font_weight] [font_size] [character_range]

Font Weight Options:
- 55: Light weight font
- 65: Regular weight font
- 75: Bold weight font

Character Range Options:
- numeric (N): Characters from . to 9
- alpha (A): Characters from A to z (and space)
- alphanumeric (AN): Characters from % to z (and space)
- f (F): Characters from f to f

Example:
python font_subsetter.py 65 12 numeric
"""
import sys
import os
import argparse
from fontTools.ttLib import TTFont
from fontTools.subset import Subsetter, Options

def get_range_details(char_range):
    """
    Get the start and end characters for the specified range.
    
    Args:
        char_range (str): The character range (numeric, alpha, or alphanumeric)
    
    Returns:
        tuple: (start_char, end_char, range_code, include_space)
    """
    # Add a flag to indicate whether space should be included
    include_space = False
    
    if char_range.lower() == "numeric" or char_range.lower() == "n":
        return '.', '9', 'N', include_space
    elif char_range.lower() == "alpha" or char_range.lower() == "a":
        include_space = True
        return 'A', 'z', 'A', include_space
    elif char_range.lower() == "alphanumeric" or char_range.lower() == "an":
        include_space = True
        return '%', 'z', 'AN', include_space
    elif char_range.lower() == "only f" or char_range.lower() == "f":
        return 'F', 'F', 'F', include_space
    else:
        raise ValueError("Invalid character range. Choose 'numeric', 'alpha', or 'alphanumeric'.")

def subset_font(font_weight, font_size, char_range):
    """
    Create a subset of a font file based on the specified parameters.
    
    Args:
        font_weight (int): Font weight (55, 65, or 75)
        font_size (int): Font size
        char_range (str): Character range (numeric, alpha, or alphanumeric)
    
    Returns:
        bool: True if successful, False otherwise
    """
    try:
        # Get the range details
        start_char, end_char, range_code, include_space = get_range_details(char_range)
        start_unicode = ord(start_char)
        end_unicode = ord(end_char)
        
        # Build the input and output file paths
        input_file = f"../resources/fonts_original/{font_weight}.otf"
        output_file = f"../resources/fonts_generated/{font_weight}w_{font_size}px_{range_code}_{start_unicode}.otf"

        if include_space:
            output_file = f"../resources/fonts_generated/{font_weight}w_{font_size}px_{range_code}_32.otf"
        
        # Ensure the directory exists
        os.makedirs(os.path.dirname(output_file), exist_ok=True)
        
        # Load the font
        font = TTFont(input_file)
        
        # Create a list of Unicode codepoints to keep
        unicodes = list(range(start_unicode, end_unicode + 1))
        
        # Add space character if needed
        if include_space:
            unicodes.append(32)  # 32 is the Unicode value for space
        
        # Create a subsetter
        options = Options()
        options.layout_features = ['*']  # Keep all layout features
        options.name_IDs = [*range(0, 7)]  # Keep important name records
        options.notdef_outline = True  # Keep the notdef glyph
        options.recalc_bounds = True
        options.recalc_timestamp = True
        options.canonical_order = True
        
        subsetter = Subsetter(options=options)
        
        # Set the Unicode characters to keep
        subsetter.populate(unicodes=unicodes)
        
        # Apply the subsetting
        subsetter.subset(font)
        
        # Save the subsetted font
        font.save(output_file)
        
        # Prepare a readable output message
        char_range_desc = f"'{start_char}' to '{end_char}'"
        if include_space:
            char_range_desc += " (including space)"
        
        print(f"Subset font created successfully: {output_file}")
        print(f"Font reduced to '{char_range}' range: {char_range_desc}")
        print(f"(Unicode range: {start_unicode} to {end_unicode}" + 
              (", plus space (32)" if include_space else "") + ")")
        return True
        
    except Exception as e:
        print(f"Error: {str(e)}")
        return False

def main():
    parser = argparse.ArgumentParser(description='Create a subset of an OTF font with predefined character ranges.')
    parser.add_argument('font_weight', choices=['55', '65', '75'], 
                        help='Font weight (55: Light, 65: Regular, 75: Bold)')
    parser.add_argument('font_size', type=int, help='Font size')
    parser.add_argument('char_range', choices=['numeric', 'alpha', 'alphanumeric', 'only F', 'N', 'A', 'AN', 'F'], 
                        help='Character range (numeric/N, alpha/A, alphanumeric/AN or only F/F)')
    
    args = parser.parse_args()
    
    # Perform the subsetting
    success = subset_font(args.font_weight, args.font_size, args.char_range)
    
    return 0 if success else 1

if __name__ == "__main__":
    sys.exit(main())