#ifndef GLOBALS_H
#define GLOBALS_H
#include "render/image_provider.h"
#include "render/verovio_loader.h"
#include "render/resvg_loader.h"
#include "render/svgrenderer.h"
#include "data_parsing/selection_handler.h"
#include "data_parsing/xml_parser.h"
#include "data_parsing/mei_parser.h"
#include "data_parsing/parser_data.h"

class globals
{
public:
    globals();
};

extern image_provider* g_image_provider; // Global image provider instance
extern verovio_loader* g_verovio_loader; // Global Verovio loader instance
extern resvg_loader* g_resvg_loader; // Global Resvg loader instance
extern selection_handler* g_selection_handler; // Global selection handler instance
extern xml_parser* g_xml_parser; // Global XML parser instance
extern mei_parser* g_mei_parser; // Global MEI parser instance
extern parser_data* g_parser_data; // Global parser data instance

#endif // GLOBALS_H
