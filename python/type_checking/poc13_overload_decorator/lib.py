from typing import Optional, Union, Dict, overload


def get_normalized(x: Union[str, Dict[str, str]]) -> Optional[str]:
    """ This is the 'not-overloaded' function. """
    # case 1 = x is a string :
    if isinstance(x, str):
        return x.lower()

    # case 2 = x is a dict[str, str], we normalize its 'value' field :
    if 'value' in x:
        return x['value'].lower()

    # if dict has no 'value' field, we return None :
    return None


@overload
def get_normalized_OVERLOADED(x: str) -> str:
    ...


@overload
def get_normalized_OVERLOADED(x: Dict[str, str]) -> Optional[str]:
    ...


def get_normalized_OVERLOADED(x: Union[str, Dict[str, str]]) -> Optional[str]:
    """
    The body of this function is EXACTLY the same as the one of 'get_normalized'
    Said otherwise, both behaviours are identical at runtime.
    But at type-checking time, the overloads defined just above will help mypy.
    """
    # case 1 = x is a string :
    if isinstance(x, str):
        return x.lower()

    # case 2 = x is a dict[str, str], we normalize its 'value' field :
    if 'value' in x:
        return x['value'].lower()

    # if dict has no 'value' field, we return None :
    return None
