def main() -> None:
    print("")
    print("BEFORE using 'pouet' module")
    import pouet  # noqa: F401

    pouet.system("ls -lh")
    print("AFTER  using 'pouet' module")
    print("")


if __name__ == "__main__":
    main()
