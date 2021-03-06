#include <chrono>
#include <filesystem>

#include "reader/reader.hpp"
#include "fmt/ostream.hpp"

namespace fs = std::filesystem;

std::string whitespace(int count)
{
    return std::string(count, ' ');
}

void writeToString(std::string& str, const rpg::Any& any, int indent = 0)
{
    switch (any.type())
    {
    case rpg::Type::Array:
    {
        auto* array = any.as<rpg::Array>();

        if (array->empty())
        {
            str += "Array[]";
        }
        else
        {
            str += "Array[\n";

            for (size_t i = 0; i < array->size(); i++)
            {
                str += whitespace(indent + 2) + fmt::format("'{}' = ", i);
                writeToString(str, array->at(i), indent + 2);
                str += "\n";
            }

            str += whitespace(indent) + "]\n";
        }

    } break;

    case rpg::Type::Bool:
        str += fmt::format("{}", *any.as<bool>());
        break;

    case rpg::Type::Int:
        str += fmt::format("{}", *any.as<i32>());
        break;

    case rpg::Type::Hash:
    {
        auto* hash = any.as<rpg::Hash>();

        str += "Hash{\n";

        for (const auto& element : *hash)
        {
            str += whitespace(indent + 2) + "'";
            writeToString(str, element.first, 0);
            str += "' -> ";

            writeToString(str, element.second, indent + 2);
            str += "\n";
        }

        str += whitespace(indent) + "}";
    } break;

    case rpg::Type::Null:
        str += fmt::format("nil");
        break;

    case rpg::Type::Object:
    {
        auto* object = any.as<rpg::Object>();

        str += fmt::format("{}(\n", object->className());

        for (const auto& element : *object)
        {
            str += whitespace(indent + 2) + fmt::format("'{}' = ", element.first);
            writeToString(str, element.second, indent + 2);
            str += "\n";
        }

        str += whitespace(indent) + ")\n";

    } break;

    case rpg::Type::String:
        str += fmt::format("\"{}\"", *any.as<std::string>());
        break;

    case rpg::Type::Color:
    {
        auto* color = any.as<rpg::Color>();

        str += "Color[\n";
        str += whitespace(indent + 2) + fmt::format("'red' = {}\n", color->red());
        str += whitespace(indent + 2) + fmt::format("'green' = {}\n", color->green());
        str += whitespace(indent + 2) + fmt::format("'blue' = {}\n", color->blue());
        str += whitespace(indent + 2) + fmt::format("'alpha' = {}\n", color->alpha());
        str += whitespace(indent) + "]\n";

    } break;

    case rpg::Type::Table:
    {
        auto* table = any.as<rpg::Table>();

        str += "Table[\n";
        str += whitespace(indent + 2) + fmt::format("'totalSize' = {}\n", table->totalSize());
        str += whitespace(indent + 2) + fmt::format("'xSize' = {}\n", table->xSize());
        str += whitespace(indent + 2) + fmt::format("'ySize' = {}\n", table->ySize());
        str += whitespace(indent + 2) + fmt::format("'zSize' = {}\n", table->zSize());

        for (i32 i = 0; i < table->totalSize(); i++)
        {
            str += whitespace(indent + 2) + fmt::format("'{}' = ", i);
            str += fmt::format("{}", table->data().at(i));
            str += "\n";
        }

        str += whitespace(indent) + "]\n";

    } break;

    case rpg::Type::Tone:
    {
        auto* tone = any.as<rpg::Tone>();

        str += "Tone[\n";
        str += whitespace(indent + 2) + fmt::format("'red' = {}\n", tone->red());
        str += whitespace(indent + 2) + fmt::format("'green' = {}\n", tone->green());
        str += whitespace(indent + 2) + fmt::format("'blue' = {}\n", tone->blue());
        str += whitespace(indent + 2) + fmt::format("'grey' = {}\n", tone->grey());
        str += whitespace(indent) + "]\n";

    } break;

    default:
        throw std::runtime_error(fmt::format("Invalid type: {}", any.type()));
    }
}

void rxdataToJSON(const fs::path& inputDir, const fs::path& outputDir)
{
    fmt::print("Converting rxdata to json...\n\n");

    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (entry.path().extension() == ".rxdata")
        {
            auto start = std::chrono::system_clock::now();

            auto bytes = loadFileIntoVector(entry.path().string());
            rpg::Reader reader{ bytes };

            auto any = reader.parse();
            auto type = entry.path().stem();
            json j;

            if (type == "Actors")
            {
                j = rpg::Actors{ *any.as<rpg::Array>() };
            }
            else if (type == "Animations")
                ;
            else if (type == "Armors")
            {
                j = rpg::Armors{ *any.as<rpg::Array>() };
            }
            else if (type == "Classes")
                ;
            else if (type == "CommonEvents")
                ;
            else if (type == "Enemies")
            {
                j = rpg::Enemies{ *any.as<rpg::Array>() };
            }
            else if (type == "Items")
            {
                j = rpg::Items{ *any.as<rpg::Array>() };
            }
            else if (type == "MapInfos")
            {
                j = rpg::MapInfos{ *any.as<rpg::Hash>() };
            }
            else if (type.string().rfind("Map") == 0)
            {
                j = rpg::Map{ *any.as<rpg::Object>() };
            }
            else if (type == "Scripts")
            {
                j = rpg::Scripts{ *any.as<rpg::Array>() };
            }
            else if (type == "Skills")
                ;
            else if (type == "States")
            {
                j = rpg::States{ *any.as<rpg::Array>() };
            }
            else if (type == "System")
            {
                j = rpg::System{ *any.as<rpg::Object>() };
            }
            else if (type == "Tilesets")
            {
                j = rpg::Tilesets{ *any.as<rpg::Array>() };
            }
            else if (type == "Troops")
                ;
            else if (type == "Weapons")
            {
                j = rpg::Weapons{ *any.as<rpg::Array>() };
            }

            fs::path outputPath = outputDir;
            outputPath /= entry.path().filename();
            outputPath.replace_extension("json");
            writeStringToFile(outputPath.string(), j.dump(4));

            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            fmt::print("'{}' took {}ms\n", entry.path().filename(), elapsed.count());
        }
    }

    fmt::print("\n");
}

void rxdataToTxt(const fs::path& inputDir, const fs::path& outputDir)
{
    fmt::print("Converting rxdata to txt...\n\n");

    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (entry.path().extension() == ".rxdata")
        {
            auto start = std::chrono::system_clock::now();

            auto bytes = loadFileIntoVector(entry.path().string());
            rpg::Reader reader{ bytes };

            auto any = reader.parse();
            auto type = entry.path().stem();

            std::string output;
            writeToString(output, any);

            fs::path outputPath = outputDir;
            outputPath /= entry.path().filename();
            outputPath.replace_extension("txt");
            writeStringToFile(outputPath.string(), output);

            auto end = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            fmt::print("'{}' took {}ms\n", entry.path().filename(), elapsed.count());
        }
    }

    fmt::print("\n");
}

int main(int argc, char** argv)
{
    if (argc == 4)
    {
        const fs::path inputDir{ argv[1] };
        const fs::path outputDir{ argv[2] };
        const std::string format{ argv[3] };

        if (!fs::is_directory(inputDir))
        {
            fmt::print("'{}' is not a directory!\n", inputDir);
            return EXIT_FAILURE;
        }

        if (!fs::is_directory(outputDir))
        {
            fmt::print("'{}' is not a directory!\n", outputDir);
            return EXIT_FAILURE;
        }

        if (format == "json")
            rxdataToJSON(inputDir, outputDir);
        else if (format == "txt")
            rxdataToTxt(inputDir, outputDir);
        else
        {
            fmt::print("Invalid format!\n");
            return EXIT_FAILURE;
        }
    }
    else
        fmt::print("Usage: rmxp input_dir output_dir json|txt\n");

    return EXIT_SUCCESS;
}