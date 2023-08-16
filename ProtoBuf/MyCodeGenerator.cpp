#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/plugin.pb.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

class MyCodeGenerator : public google::protobuf::compiler::CodeGenerator {
public:
    MyCodeGenerator() {}
    virtual ~MyCodeGenerator() {}

    bool Generate(const google::protobuf::FileDescriptor* file, const std::string& parameter, google::protobuf::compiler::GeneratorContext* generator_context, std::string* error) const override 
    {
        std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> output(generator_context->Open(file->name() + ".txt"));
        google::protobuf::io::Printer printer(output.get(), '$');
        for (int i = 0; i < file->message_type_count(); i++)
        {
            auto MessageType = file->message_type(i);
            if (MessageType)
            {
                printer.Print(MessageType->name().c_str());
                printer.Print("\n");
            }
        }
        printer.Print("This is a placeholder for generated code.\n");
        return true;
    }
};

int TestCodeGenerator(int argc, char* argv[])
{
    std::this_thread::sleep_for(std::chrono::seconds(10));
    MyCodeGenerator generator;
    return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}