#!/usr/bin/env python3

from lib import get_normalized, get_normalized_OVERLOADED


if __name__ == "__main__":
    print("""
    The intent of the POC is to illustrate the purpose of 'typing.overload' decorator :
        https://docs.python.org/3/library/typing.html#typing.overload

    THE POC :
    - a function 'get_normalized' that expects either a string or a dict, and returns a string or None
    - when get_normalized is given a dict, it can return None
    - when get_normalized is given a string, it NEVER returns None
    - the POC calls 'get_normalized' on a string, then appends a SUFFIX to its result

    WHY IS typing.overload USEFUL ?
    - usually, get_normalized would be type-hinted as 'returning an Optional[str]'.
    - but as we pass a string to get_normalized, we are SURE that the returned value is a string and not None
    - problem : the type-checker cannot know that, and cannot know that it can safely append a string SUFFIX...
    - thus, mypy is RED on the SUFFIX append for 'get_normalized'
    - in contrario, overload allows us to tell mypy 'when this function is called on a string, it ALWAYS returns str'
    - thus mypy is GREEN on the SUFFIX append for 'get_normalized_OVERLOADED'

    TL;DR : 'overload' is useful when the return-type is different according to a particular parameter-type.

    BUT I HAVE CONCERNS :
    - first, it's (often) a bad idea to accept various parameter-types, and change the returned type accordingly...
    - it seems that the body of the function is not type-checked anymore ? mypy would not detect if I return 42 ?!
      EDIT : actually, it seems that the body CAN be type-checked. This is in opposition with what the doc says :
         A series of @overload-decorated definitions must be followed by exactly one non-@overload-decorated definition
         ...the latter is used at runtime but should be ignored by a type checker
    - even if it was type-checked, nothing enforces what the overload describes ?!
      (i.e. nothing — and particularly not mypy — detects if 'get_normalized' returns None when given a string)
    """)

    # this part is just here to illustrate how 'get_normalized' works :
    #   - either directly on a string
    #   - or on a dict which has a 'value' element
    # If the dict has no 'value' element, get_normalized returns None.
    # In all the other cases, get_normalized returns the lowercase string.
    name_str = "Luke SKYWALKER"
    name_dict_ok = {"value": "Leia SKYWALKER"}
    name_dict_ko = {"dummy": "pouet"}

    normalized_str = get_normalized(name_str)  # luke skywalker
    normalized_dict_ok = get_normalized(name_dict_ok)  # leia skywalker
    normalized_dict_ko = get_normalized(name_dict_ko)  # None

    print(f"Normalizing '{name_str}' returned '{normalized_str}'")
    print(f"Normalizing '{name_dict_ok}' returned '{normalized_dict_ok}'")
    print(f"Normalizing '{name_dict_ko}' returned '{normalized_dict_ko}'")

    # the interest of the POC is here : to show that the non-overloaded call trigs a mypy error :
    SUFFIX = ".txt"
    full_name_str1 = get_normalized("POUET1") + SUFFIX  # a mypy error is expected here...
    full_name_str2 = get_normalized_OVERLOADED("POUET2") + SUFFIX  # ... but not here
    print(f"Full name is : '{full_name_str1}'")
    print(f"Full name is : '{full_name_str2}'")
