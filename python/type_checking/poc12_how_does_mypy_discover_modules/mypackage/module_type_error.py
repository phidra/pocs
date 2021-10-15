# in this module, the function has a bug, which is statically detectable (as it results in a type-annotation mismatch)
def get_name_length(name: str) -> int:
    return name  # bugged : should be 'return len(name)'
