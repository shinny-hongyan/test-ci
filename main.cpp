#include <iostream>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

int main()
{
    // 1. 创建 JSON 对象
    Document document;
    document.SetObject();

    // 创建分配器，用于分配内存
    Document::AllocatorType &allocator = document.GetAllocator();

    // 添加简单的键值对
    document.AddMember("name", "张三", allocator);
    document.AddMember("age", 30, allocator);
    document.AddMember("isStudent", false, allocator);

    // 添加数组
    Value hobbies(kArrayType);
    hobbies.PushBack("读书", allocator);
    hobbies.PushBack("游泳", allocator);
    hobbies.PushBack("编程", allocator);
    document.AddMember("hobbies", hobbies, allocator);

    // 添加嵌套对象
    Value address(kObjectType);
    address.AddMember("city", "北京", allocator);
    address.AddMember("street", "朝阳区中关村", allocator);
    address.AddMember("postcode", "100000", allocator);
    document.AddMember("address", address, allocator);

    // 2. 将 JSON 对象序列化为字符串
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    // 输出序列化后的 JSON 字符串
    cout << "生成的 JSON:" << endl;
    cout << buffer.GetString() << endl
         << endl;

    // 3. 解析 JSON 字符串
    const char *json = R"({"name":"李四","age":25,"skills":["C++","Python","JavaScript"]})";
    Document parsedDoc;
    parsedDoc.Parse(json);

    // 4. 访问 JSON 对象的值
    cout << "解析的 JSON:" << endl;

    // 检查是否为对象
    if (parsedDoc.IsObject())
    {
        // 访问简单类型
        if (parsedDoc.HasMember("name") && parsedDoc["name"].IsString())
        {
            cout << "姓名: " << parsedDoc["name"].GetString() << endl;
        }

        if (parsedDoc.HasMember("age") && parsedDoc["age"].IsInt())
        {
            cout << "年龄: " << parsedDoc["age"].GetInt() << endl;
        }

        // 访问数组
        if (parsedDoc.HasMember("skills") && parsedDoc["skills"].IsArray())
        {
            const Value &skills = parsedDoc["skills"];
            cout << "技能: ";
            for (SizeType i = 0; i < skills.Size(); i++)
            {
                if (i > 0)
                    cout << ", ";
                cout << skills[i].GetString();
            }
            cout << endl;
        }
    }

    // 5. 修改 JSON 对象
    if (parsedDoc.HasMember("age"))
    {
        parsedDoc["age"].SetInt(parsedDoc["age"].GetInt() + 1);
    }

    // 添加新成员
    parsedDoc.AddMember("graduate", true, parsedDoc.GetAllocator());

    // 6. 再次序列化
    StringBuffer modifiedBuffer;
    Writer<StringBuffer> modifiedWriter(modifiedBuffer);
    parsedDoc.Accept(modifiedWriter);

    cout << endl
         << "修改后的 JSON:" << endl;
    cout << modifiedBuffer.GetString() << endl;

    return 0;
}