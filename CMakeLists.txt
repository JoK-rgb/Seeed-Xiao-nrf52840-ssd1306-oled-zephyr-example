cmake_minimum_required(VERSION 3.20.0)
set(BOARD seeed_xiao_nrf52840)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_ssd1306)

target_sources(app PRIVATE src/main.c)