#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <vector>
#include <string>
#include <bitset>
#include <QLabel>
#include <QDebug>


std::string show_vect(const std::vector<std::string> &vect)
{
    std::string tempstr;
    for (size_t i = 0; i < vect.size()-1; i++)
    {
        tempstr += vect[i] + '.';
    }
    tempstr += vect[vect.size() - 1];
    return tempstr;
}


std::vector<std::string> binary_addr(const std::vector<std::string>& vect)
{
    std::vector<std::string> tempvect;
    for (size_t i = 0; i < vect.size()-1; i++)
    {
        tempvect.push_back((std::bitset<8>(std::stoi(vect[i])).to_string()));
    }
    tempvect.push_back(std::bitset<8>(std::stoi(vect[vect.size()-1])).to_string());
    return tempvect;
}

std::vector<std::string> input_string_to_vector(std::string input_string)
{
    std::vector<std::string> ipv4;
    while (!input_string.empty())
    {
        std::string temp_for_ipv4 = input_string.substr(0, input_string.find('.'));
        ipv4.push_back(temp_for_ipv4);
        input_string.erase(input_string.begin(), (std::find(input_string.begin(), (input_string.end() - 1), '.') + 1));
    }
    return ipv4;
}

std::vector<std::string> binary_mask(int mask)
{
    std::vector<std::string> tempvect;
    for (int i = 0; i < 4; i++)
    {
        tempvect.push_back(std::bitset<8>(0).to_string());
    }

    for (int i = 0; i < 32; i++)
    {
        if (i < mask)
            tempvect[i / 8][i % 8] = '1';
    }
    return tempvect;
}

int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;

        dec_value += last_digit * base;

        base = base * 2;
    }

    return dec_value;
}

std::vector<std::string> binary_to_decim(const std::vector<std::string>& vect)
{
    std::vector<std::string> tempvect;
    for (size_t i = 0; i < vect.size(); i++)
    {
        tempvect.push_back(std::to_string(binaryToDecimal(std::stoi(vect[i]))));
    }
    return tempvect;
}

std::vector<std::string> minimal(const std::vector<std::string>& addr,int mask)
{
    std::vector<std::string> tempvect;
    std::vector<std::string> mask_binary = binary_mask(mask);
    std::vector<std::string> addr_binary = binary_addr(addr);
    for (size_t i = 0; i < 4; i++)
    {
        tempvect.push_back (std::bitset<8>(binaryToDecimal(std::stoi(addr_binary[i])) & binaryToDecimal(std::stoi(mask_binary[i]))).to_string());
    }
    //show(tempvect);
    tempvect[tempvect.size() - 1] = std::bitset<8>(std::stoi(tempvect[tempvect.size() - 1]) + 1).to_string();
    return tempvect;
}

std::vector<std::string> maximal(const std::vector<std::string>& vect,int mask)
{
    std::vector<std::string> tempvect=vect;
    for (size_t i = mask; i < 31; i++)
    {
        tempvect[i / 8][i % 8] = '1';
    }
    return tempvect;
}

std::vector<std::string> mask_addr(const std::vector<std::string> &vect)
{
    std::vector<std::string> tempvect;
    for (size_t i = 0; i < vect.size(); i++)
    {
        tempvect.push_back(std::to_string(std::bitset<8>((binary_mask(24)[i])).to_ulong()));
    }
    return tempvect;
}

std::string ip_class(const std::vector<std::string> &vect)
{
    std::string tmp=binary_addr(vect)[0];
    int klass=0;
    int prev_klass=0;
    for(size_t i=0;i<4;i++)
    {
        if(tmp[i]=='1')
           klass++;
        if(klass==prev_klass)
            break;
        prev_klass=klass;
    }
    switch(klass)
    {
    case 0:return "A";
    case 1:return "B";
    case 2:return "C";
    case 3:return "D";
    case 4:return "E";
    default:return "A";
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty()){
    //ui->comboBox->setCurrentIndex(23);
    std::vector<std::string> ipv4=input_string_to_vector(ui->lineEdit->text().toUtf8().constData());
    std::string temp_string_ipv4=(ui->lineEdit->text()).toUtf8().constData();
    //IPv4
    ui->label->setText("IPv4 - "+QString::fromStdString(temp_string_ipv4));
    //Binary ipv4
    ui->label_2->setText("Binary IPv4 - "+QString::fromStdString(show_vect(binary_addr(ipv4))));
    //Mask
    ui->label_3->setText("Netmask - "+QString::fromStdString(show_vect(mask_addr(binary_mask(ui->comboBox->currentIndex()+1)))));
    //Mask Binary
    ui->label_4->setText("NetMask Binary - "+QString::fromStdString(show_vect(binary_mask(ui->comboBox->currentIndex()))));

   //Host min "HostMin - "
    ui->label_5->setText("HostMin -"+QString::fromStdString(show_vect(binary_to_decim(minimal(ipv4,ui->comboBox->currentIndex())))));
    //Host max "HostMax - "
    ui->label_6->setText("HostMax - "+QString::fromStdString(show_vect(binary_to_decim(maximal(binary_addr(ipv4),ui->comboBox->currentIndex())))));
    //class ipv4
    ui->label_7->setText("Class: "+QString::fromStdString(ip_class(ipv4)));
}
}


