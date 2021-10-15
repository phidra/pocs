#!/usr/bin/env python3

# this imported module has no bug :
import mypackage.module_type_ok

# hence, the call here has no bug either :
luke = "Luke SKYWALKER"
print("main_ok : returned = ", mypackage.module_type_ok.get_lower_name(luke))
