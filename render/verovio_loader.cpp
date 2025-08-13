#include "verovio_loader.h"
#include "toolkit.h"

using namespace vrv;

verovio_loader::verovio_loader(QObject *parent)
    : QObject{parent}
{
    tk = new Toolkit(true); // point the pointer to a new toolkit instance
    QDir dir(QDir::currentPath());
    dir.cdUp();
    dir.cdUp();
    verovio_resource_path = dir.path() + "/build/Desktop_Qt_6_9_1_MSVC2022_64bit-Release/external/verovio/data";

    options = R"({
        "pageHeight": 5000,
        "pageWidth": 3000,
        "pageMarginLeft": 50,
        "pageMarginRight": 50,
        "pageMarginTop": 0,
        "pageMarginBottom": 50,
        "footer": "none",
        "header": "none",
        "spacingSystem": 10,
        "adjustPageHeight": true,
        "adjustPageWidth": false,
        "breaks": "encoded",
        "justifyVertically": false,
        "condense": "none",
        "spacingLinear": 0.8,
        "spacingNonLinear": 0.3,
    })";

    tk->SetOptions (options.toStdString ());
    tk->SetResourcePath (verovio_resource_path.toStdString ());


}

void verovio_loader::load(const QString &target_path)
{
    tk->SetOptions (options.toStdString ());
    tk->LoadFile(target_path.toStdString());
}

void verovio_loader::load_data(const QString &mei_data)
{
    //tk->SetOptions (options.toStdString ());
    tk->LoadData(mei_data.toStdString());
}

std::string verovio_loader::render(const int &page_number)
{
    //tk->RenderToSVGFile("C:/Users/david/Downloads/test 1/" + std::to_string (page_number)+".svg", page_number);
    return tk->RenderToSVG (page_number);
}

int verovio_loader::get_page_count()
{
    return tk->GetPageCount();
}

std::string verovio_loader::get_mei_data()
{
    std::string mei_data = tk->GetMEI();
    return mei_data;
}



