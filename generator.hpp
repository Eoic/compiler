#include <stack>
#include <iomanip>
#include <llvm-7/llvm/IR/Module.h>
#include <llvm-7/llvm/IR/LLVMContext.h>
#include <llvm-7/llvm/ExecutionEngine/GenericValue.h>
#include <llvm-7/llvm/IR/Instructions.h>
#include <llvm-7/llvm/IR/LegacyPassManager.h>
#include <llvm-7/llvm/IR/IRPrintingPasses.h>
#include <llvm-7/llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm-7/llvm/IR/IRBuilder.h>
#include <llvm-7/llvm/Support/Casting.h>

class Block;

static llvm::LLVMContext llvmContext;
static llvm::IRBuilder<> builder(llvmContext);

/**
 * Code block which contains basi block type from LLVM,
 * return value and map of local variables.
 */
class GeneratorBlock
{
public:
    llvm::BasicBlock *block;
    llvm::Value *returnValue;
    std::map<std::string, llvm::Value *> locals;
};

class GeneratorContext
{
    std::stack<GeneratorBlock *> blocks;
    llvm::Function *mainFunction;
    bool verboseOutput;
    int logNumber = 0;

public:
    llvm::Module *module;

    GeneratorContext(bool verboseOutput)
    {
        module = new llvm::Module("main", llvmContext);
        this->verboseOutput = verboseOutput;
    }

    void compileModule(Block &root);

    llvm::GenericValue runCode();

    std::map<std::string, llvm::Value *> &locals()
    {
        return blocks.top()->locals;
    }

    llvm::BasicBlock *currentBlock()
    {
        return blocks.top()->block;
    }

    void pushBlock(llvm::BasicBlock *block)
    {
        GeneratorBlock *generatorBlock = new GeneratorBlock();
        blocks.push(generatorBlock);
        blocks.top()->returnValue = NULL;
        blocks.top()->block = block;
    }

    void popBlock()
    {
        GeneratorBlock *top = blocks.top();
        blocks.pop();
        delete top;
    }

    void setCurrentReturnValue(llvm::Value *value)
    {
        blocks.top()->returnValue = value;
    }

    llvm::Value* getCurrentReturnValue() 
    {
        return blocks.top()->returnValue;
    }

    void logMessage(const std::string message)
    {
        if(verboseOutput) {
            std::string id = std::to_string(++logNumber) + ".";
            std::cout << std::left << std::setw(5) << id << message << std::endl;
        }
    }
};