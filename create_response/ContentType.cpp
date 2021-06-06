#include "ContentType.hpp"

std::string    ContentType::_type_text[6] = {"css", "csv", "html", "javascript", "plain", "xml"};

std::string    ContentType::_type_audio[4] = {"mpeg", "x-ms-wma", "vnd.rn-realaudio", "x-wav"};

std::string    ContentType::_type_video[7] = {"mpeg", "mp4", "quicktime",
                                                "x-ms-wmv", "x-msvideo", "x-flv", "webm"};

std::string    ContentType::_type_image[8] = {"gif", "jpeg", "png", "tiff",
                                                "vnd.microsoft.icon", "x-icon", "vnd.djvu", "svg+xml"};

std::string    ContentType::_type_multipart[4] = {"mixed", "alternative", "related", "form-data"};

std::string    ContentType::_type_application[14] = {"java-archive", "EDI-X12",
                                                    "EDIFACT", "javascript", "octet-stream",
                                                    "ogg", "pdf", "xhtml+xml", "x-shockwave-flash",
                                                    "json", "ld+json", "xml", "zip", "x-www-form-urlencoded"};

ContentType::ContentType() {

}

ContentType::~ContentType() {

}

std::string          ContentType::search_type(string expansion, string *types) {
    for (int i = 0; i < types->size(); i++){
        if (expansion == types[i])
            return (types[i]);
    }
    return ("NULL");
}

std::string           ContentType::get_type(string expansion) {
    string  type;

    if ((type = search_type(expansion, _type_text)) != "NULL"){
        return ("text/" + type);
    }
    if ((type = search_type(expansion, _type_audio)) != "NULL"){
        return ("audio/" + type);
    }
    if ((type = search_type(expansion, _type_image)) != "NULL"){
        return ("image/" + type);
    }
    if ((type = search_type(expansion, _type_video)) != "NULL"){
        return ("video/" + type);
    }
    if ((type = search_type(expansion, _type_multipart)) != "NULL"){
        return ("multipart/" + type);
    }
    if ((type = search_type(expansion, _type_application)) != "NULL"){
        return ("application/" + type);
    }
    return (type);
}
