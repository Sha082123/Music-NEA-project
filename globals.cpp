#include "globals.h"

globals::globals() {}

image_provider* g_image_provider = nullptr; // Initialize the global image provider pointer to nullptr
verovio_loader* g_verovio_loader = nullptr; // Initialize the global Verovio loader pointer to nullptr
resvg_loader* g_resvg_loader = nullptr; // Initialize the global Resvg loader pointer to nullptr
xml_parser* g_xml_parser = nullptr; // Initialize the global XML parser pointer to nullptr
mei_parser* g_mei_parser = nullptr; // Initialize the global MEI parser pointer to nullptr
parser_data* g_parser_data = nullptr; // Initialize the global parser data pointer to nullptr
