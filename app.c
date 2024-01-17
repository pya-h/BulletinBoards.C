#include <stdio.h>
#include "lib/user.h"

int main() {
    User *user = registerUser("Pedram", "test");

    puts(user->location);
    
}