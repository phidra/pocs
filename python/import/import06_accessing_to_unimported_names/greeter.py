I18N_GREETINGS = {
    "en_GB": "Hello",
    "fr_FR": "Salut",
}


def do_greet(arg: str, language_code: str = "en_GB"):
    try:
        greetings = I18N_GREETINGS[language_code]
        print(f"{greetings} '{arg}'")
    except KeyError:
        print(f"I can't greet '{arg}' in this language : {language_code}")
