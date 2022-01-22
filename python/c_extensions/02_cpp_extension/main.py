import pouet


def main() -> None:
    statement = "Luke Skywalker is a jedi"
    nb_chars = pouet.count_nb_chars(statement)
    assert nb_chars == len(statement)
    print(f"OK : statement with {nb_chars} chars was : '{statement}'")


if __name__ == "__main__":
    main()
