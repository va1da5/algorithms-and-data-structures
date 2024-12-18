import logging

logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)


def main() -> None:
    logger.debug("Hello, World!")


if __name__ == "__main__":
    main()
