#ifndef __EULER_059_H__
#define __EULER_059_H__

#include "basic_headers.h"
/*
* ֮ǰ����Ŀ����������⵼�¸��ֵò������~~~~ֻ�ò������ϵĽⷨ���ŷ���������~
* ����Ȿ��Ҳ�и����⣬ֻ��˵���ܺ���ı�����Ӣ�ĵ��ʣ��ܲ��Ͻ�
* Ӧ��ֱ��˵���ܵ��ı���һ��Ӣ�����¶��䣬��������Ӣ�ĵ��ʿ��԰���������һЩ�ַ�������ո� ��� ����֮��ģ�
* ��Ȼ����ô֪���������ʵ���˼��ÿһ�������Ƿֿ��ģ��һ���Ϊȫ����Ӣ����ĸ�����е�һ��Ƭ���ǵ��ʾͿ�����~
* ��˵�ˣ����ʱ���Ҳ���Ǻ����ܵĸ����
* �����Ұ�����ţ��ʵ�����е��ʶ����ȱ�������������һ�����ȶԣ���Ȼ����ô�������ж�֮��ķ�ʽ֪������Ƿ���ȷ��~
* ���⣬���˵��һ�����£���ô����ʹ��ÿ���ַ��ĳ���Ƶ�ʣ�����ո��Ƶ�ʻ�ܸߣ����� e�ĳ���Ƶ�ʻ�ܸߣ�����������
* ����û��������˵�������ּ����ȱ����֤
* ���һ�㣬��Ϊ�����⣬����Ƿ������£����߰������ʣ�������Ҫ�˹��жϵģ������޷�д���ɿ������жϽ���Ƿ���õķ�ʽ
* �����ⲻ��һ�����������ϵı���⣬����ֻ�ǰѻ�е�����������һ�£�������ֻ�ܿ����Լ��ж��Ƿ����
*/

size_t translate(const char *content, size_t len, char *dest) {
    int ascii_val = 0;
    const char* p = content;
    size_t pos = 0;
    while (*p != '\0' && p-content < len) {
        if ('0' <= *p && *p <= '9') {
            ascii_val = ascii_val * 10 + (*p - '0');
        }
        else {
            dest[pos++] = (char)ascii_val;
            ascii_val = 0;
        }
        ++p;
    }

    dest[pos] = '\0';
    return pos;
}

bool decrypt(const char *content, size_t len, char* dest, const char* key) {
    // key is 3-lower case charactors word
    //content is end with '\0'
    //dest is large enough ( >= strlen(content) )
    const char* p_content = content;
    size_t pos = 0;
    while (pos < len) {
        int r = pos % 3;
        
        dest[pos] = *p_content ^ key[r];

        if (!isprint(dest[pos])) {
            return false;
        }
        ++p_content;
        ++pos;
    }
    return true;
}


void resolution() {
    //read file 
    string file_name = "p059_cipher.txt";
    ifstream ifile(file_name);
    if (ifile.is_open()) {
        char original_content[4096] = { 0 };  //�����ļ�������ı�
        ifile.read(original_content, sizeof(original_content));

        char encrypted_content[2048] = { 0 };
        size_t len = translate(original_content, sizeof(original_content), encrypted_content);
        cout << "encrypted_content len is " << len << endl;
        char plain_text[2048] = { 0 };  //ԭʼ���ݣ����ܳ������ı�

        string output_file_name = "original.txt";
        ofstream ofile(output_file_name);
        if (!ofile.is_open()) {
            cout << "can't create/open output file : " << output_file_name << endl;
            ifile.close();
            return;
        }
        char output_content[64] = { 0 };

        long sum_of_ascii = 0;
        char encryption_key[4] = { 0 }; //���룺��������Сд��ĸ
        for (encryption_key[0] = 'a'; encryption_key[0] <= 'z'; ++encryption_key[0]) {
            for (encryption_key[1] = 'a'; encryption_key[1] <= 'z'; ++encryption_key[1]) {
                for (encryption_key[2] = 'a'; encryption_key[2] <= 'z'; ++encryption_key[2]) {
                    if (encryption_key[0] == 'g' && encryption_key[1] == 'o' && encryption_key[2] == 'd')
                    {
                        cout << "test ..." << endl;
                    }

                    if (decrypt(encrypted_content, len, plain_text, encryption_key)) {
                        cout << "------------------------"
                            << "\nkey is " << encryption_key 
                            << "\ntarget is "
                            << plain_text[0] << plain_text[1] << plain_text[2] << plain_text[3] << plain_text[4] << plain_text[5]
                            << plain_text[6] << plain_text[7] << plain_text[8] << plain_text[9] << plain_text[10] << plain_text[11]
                            << plain_text[12] << plain_text[13] << plain_text[14] << plain_text[15] << plain_text[16] << plain_text[17]
                            << "...\n\n";

                        for (const char* p = plain_text; *p != '\0'; ++p) {
                            sum_of_ascii += (long)(*p);
                        }
                        
                        sprintf_s(output_content, sizeof(output_content),
                            "key is %s.\nsum of content is %ld.\n",
                            encryption_key, sum_of_ascii);
                        ofile.write(output_content, strlen(output_content));
                        ofile.write(plain_text, strlen(plain_text));
                        ofile.write("\n\n", 2);

                        sum_of_ascii = 0;
                    }
                }
            }
        }
        ofile.close();
        ifile.close();
    }
    else {
        cout << "can't open file " << file_name << endl;
    }
}


#endif /*__EULER_059_H__*/