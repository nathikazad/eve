import json
import argparse

def move_ram(start_ram, offset):
    with open('../editor_files/5/assets.ese', 'r') as file:
        assets = json.load(file)
        contents = assets['content']
    for content in contents:
        if content['converter'] == 'Font':
            print(content['memoryAddress'])
            if content['memoryAddress'] > start_ram:
                content['memoryAddress'] += offset
def main():
    parser = argparse.ArgumentParser(description='Move all font after a certain ram address to a new ram address by a certain offset')
    parser.add_argument('start_ram', type=int, help='Start ram address')
    parser.add_argument('offset', type=int, help='Offset')
    args = parser.parse_args()
    move_ram(args.start_ram, args.offset)

if __name__ == "__main__":
    main()