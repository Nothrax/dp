#!/usr/bin/env python3
import logging

from flask_endpoint import FlaskEndpoint
from flask_endpoint import argparse_init
from flask_endpoint.config import load_config


def main() -> None:
    logging.basicConfig(
        level=logging.INFO,
        format="%(name)s: %(message)s",
        datefmt="[%X]",
    )
    logger = logging.getLogger("Main")
    args = argparse_init()
    config = load_config(args.config)

    server = FlaskEndpoint(config)
    try:
        logger.info("Starting uploader")
        server.start()
    except KeyboardInterrupt:
        logger.info("Stopping uploader")
        server.stop()


if __name__ == "__main__":
    main()
