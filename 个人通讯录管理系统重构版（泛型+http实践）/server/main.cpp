//server.cpp
#include <iostream>
#include "httplib.h"
using namespace std;
void upload_handler(const httplib::Request& req, httplib::Response& res) {
    //检查请求方法是否为 POST
    cout << "收到文件上传请求";
    if (req.method == "POST") {
        //获取上传文件的信息
        auto file = req.get_file_value("ContatcsDataText");
        //获取上传的文件名
        string original_filename = file.filename;
        //在这里你可以处理接收到的文本文件内容，并保存文件
        ofstream ofs(original_filename, ios::binary);
        ofs.write(file.content.c_str(), file.content.length());
        ofs.close();
        //返回上传成功的响应
        res.status = 200;
        res.set_content("文件上传成功！文件名：" + original_filename, "text/plain");
        cout << "，文件上传成功！文件名：" << original_filename << endl;
    }
    else {
        //返回不支持的请求方法的响应
        res.status = 405;
        res.set_content("不支持的请求方法", "text/plain");
        cout << "，文件上传失败！不支持的请求方法。" << endl;
    }
    return;
}
void download_handler(const httplib::Request& req, httplib::Response& res) {
    cout << "收到文件下载请求";
    //从请求路径中获取文件名
    string path = req.path;
    string filename = path.substr(path.find_last_of("/") + 1);
    //读取文件内容
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        //设置响应头，指示文件下载
        res.set_header("Content-Disposition", "attachment; filename=" + filename);
        //设置响应内容和 Content-Type
        res.set_content(content, "application/octet-stream");
        res.status = 200;
        cout << "，已为客户传输文件，文件名：" << filename << endl;
    }
    else {
        //文件不存在或无法打开
        res.status = 404;
        cout << "，无法提供文件，文件未找到" << endl;
    }
    return;
}
void remove_file(const httplib::Request& req, httplib::Response& res) {
    cout << "收到文件删除请求";
    //从请求路径中提取文件名
    string path = req.path;
    string filename = path.substr(path.find_last_of("/") + 1);
    //删除本地文件
    if (remove(filename.c_str()) == 0) {
        res.status = 200;
        res.set_content("文件删除成功", "text/plain");
        cout << "，文件删除成功，文件名：" << filename << endl;
    }
    else {
        res.status = 404;
        res.set_content("文件删除失败，文件可能不存在", "text/plain");
        cout << "，文件删除失败，文件名：" << filename << "可能不存在" << endl;
    }
    return;
}
int main() {
    //创建并配置 HTTP 服务器
    httplib::Server server;
    //处理 POST 请求的 "/upload" 路由
    server.Post("/upload", upload_handler);
    //处理 GET 请求的 "/download/*" 路由，* 表示通配符，用于捕获后续的路径部分
    server.Get(R"(/download/(.*))", [&](const httplib::Request& req, httplib::Response& res) {
        download_handler(req, res);
        });
    //处理 GET 请求的 "/remove/*" 路由，* 表示通配符，用于捕获后续的路径部分
    server.Get("/remove/(.*)", remove_file);
    //启动服务器并监听端口
    cout << "个人通讯录管理系统-服务器已启动，正在监听端口：19060" << endl;
    server.listen("0.0.0.0", 19060);
    return 0;
}
