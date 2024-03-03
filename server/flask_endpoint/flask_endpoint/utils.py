import argparse


def argparse_init() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-c", "--config", type=str, default="./config/default.json", help="Path to the json config file"
    )
    return parser.parse_args()
