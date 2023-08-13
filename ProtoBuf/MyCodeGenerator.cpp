#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/plugin.pb.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

class MyCodeGenerator : public google::protobuf::compiler::CodeGenerator {
public:
    MyCodeGenerator() {}
    virtual ~MyCodeGenerator() {}

    bool Generate(const google::protobuf::FileDescriptor* file,
        const std::string& parameter,
        google::protobuf::compiler::GeneratorContext* generator_context,
        std::string* error) const override {
        // 对于每个.proto文件，生成一个输出文件
        std::unique_ptr<google::protobuf::io::ZeroCopyOutputStream> output(
            generator_context->Open(file->name() + ".txt"));
        google::protobuf::io::Printer printer(output.get(), '$');
        printer.Print("This is a placeholder for generated code.\n");
        return true;
    }
};

int TestCodeGenerator(int argc, char* argv[])
{
    MyCodeGenerator generator;
    return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}