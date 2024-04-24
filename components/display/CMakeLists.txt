idf_component_register(SRCS "display.c" "lcd.c"
                       INCLUDE_DIRS "."
                       REQUIRES esp_lcd)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wall -Werror -Wextra -Wshadow -Wsign-compare -Wunreachable-code)
