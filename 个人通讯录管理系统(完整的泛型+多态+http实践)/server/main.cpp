//server.cpp
#include <iostream>
#include "httplib.h"
using namespace std;
void upload_handler(const httplib::Request& req, httplib::Response& res) {
    //������󷽷��Ƿ�Ϊ POST
    cout << "�յ��ļ��ϴ�����";
    if (req.method == "POST") {
        //��ȡ�ϴ��ļ�����Ϣ
        auto file = req.get_file_value("ContatcsDataText");
        //��ȡ�ϴ����ļ���
        string original_filename = file.filename;
        //����������Դ�����յ����ı��ļ����ݣ��������ļ�
        ofstream ofs(original_filename, ios::binary);
        ofs.write(file.content.c_str(), file.content.length());
        ofs.close();
        //�����ϴ��ɹ�����Ӧ
        res.status = 200;
        res.set_content("�ļ��ϴ��ɹ����ļ�����" + original_filename, "text/plain");
        cout << "���ļ��ϴ��ɹ����ļ�����" << original_filename << endl;
    }
    else {
        //���ز�֧�ֵ����󷽷�����Ӧ
        res.status = 405;
        res.set_content("��֧�ֵ����󷽷�", "text/plain");
        cout << "���ļ��ϴ�ʧ�ܣ���֧�ֵ����󷽷���" << endl;
    }
    return;
}
void download_handler(const httplib::Request& req, httplib::Response& res) {
    cout << "�յ��ļ���������";
    //������·���л�ȡ�ļ���
    string path = req.path;
    string filename = path.substr(path.find_last_of("/") + 1);
    //��ȡ�ļ�����
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        //������Ӧͷ��ָʾ�ļ�����
        res.set_header("Content-Disposition", "attachment; filename=" + filename);
        //������Ӧ���ݺ� Content-Type
        res.set_content(content, "application/octet-stream");
        res.status = 200;
        cout << "����Ϊ�ͻ������ļ����ļ�����" << filename << endl;
    }
    else {
        //�ļ������ڻ��޷���
        res.status = 404;
        cout << "���޷��ṩ�ļ����ļ�δ�ҵ�" << endl;
    }
    return;
}
void remove_file(const httplib::Request& req, httplib::Response& res) {
    cout << "�յ��ļ�ɾ������";
    //������·������ȡ�ļ���
    string path = req.path;
    string filename = path.substr(path.find_last_of("/") + 1);
    //ɾ�������ļ�
    if (remove(filename.c_str()) == 0) {
        res.status = 200;
        res.set_content("�ļ�ɾ���ɹ�", "text/plain");
        cout << "���ļ�ɾ���ɹ����ļ�����" << filename << endl;
    }
    else {
        res.status = 404;
        res.set_content("�ļ�ɾ��ʧ�ܣ��ļ����ܲ�����", "text/plain");
        cout << "���ļ�ɾ��ʧ�ܣ��ļ�����" << filename << "���ܲ�����" << endl;
    }
    return;
}
int main() {
    //���������� HTTP ������
    httplib::Server server;
    //���� POST ����� "/upload" ·��
    server.Post("/upload", upload_handler);
    //���� GET ����� "/download/*" ·�ɣ�* ��ʾͨ��������ڲ��������·������
    server.Get(R"(/download/(.*))", [&](const httplib::Request& req, httplib::Response& res) {
        download_handler(req, res);
        });
    //���� GET ����� "/remove/*" ·�ɣ�* ��ʾͨ��������ڲ��������·������
    server.Get("/remove/(.*)", remove_file);
    //�����������������˿�
    cout << "����ͨѶ¼����ϵͳ-�����������������ڼ����˿ڣ�19060" << endl;
    server.listen("0.0.0.0", 19060);
    return 0;
}
