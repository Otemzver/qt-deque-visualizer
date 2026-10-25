#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <cwctype>
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    
    std::random_device rd;
    std::mt19937 gen_shuffle(rd());
    SetRandomGen(gen_shuffle);

    ApplyModel();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::ApplyModel() {
    auto preserve_iter = deque_model_.iterator;
    ui->list_widget->clear();

    for (size_t i = 0; i < deque_model_.items.size(); ++i) {
        QString itemText = QString::number(i) + ": ";
        if (!deque_model_.items[i].empty()) {
            itemText += QString::fromStdString(deque_model_.items[i]);
        }
        ui->list_widget->addItem(itemText);
    }
    ui->list_widget->addItem("end");
    ui->txt_size->setText(QString::number(deque_model_.items.size()));
    ui->pb_pop_back->setEnabled(!deque_model_.items.empty());
    ui->pb_pop_front->setEnabled(!deque_model_.items.empty());
    deque_model_.iterator = preserve_iter;
    ApplyIterator();
}

void MainWindow::ApplyIterator() {
    if (deque_model_.iterator < deque_model_.items.begin() || deque_model_.items.end() < deque_model_.iterator) {
        return;
    }

    auto dst = std::distance(deque_model_.items.begin(), deque_model_.iterator);
    ui->list_widget->setCurrentRow(dst);

    bool is_begin_iterator = (deque_model_.iterator == deque_model_.items.begin());
    bool is_end_iterator = (deque_model_.iterator == deque_model_.items.end());

    ui->pb_edit->setEnabled(!is_end_iterator);
    ui->pb_erase->setEnabled(!is_end_iterator);
    ui->pb_iterator_dec->setEnabled(!is_begin_iterator);
    ui->pb_iterator_inc->setEnabled(!is_end_iterator);

    ui->txt_elem_content->setText(is_end_iterator
        ? ""
        : QString::fromStdString(*(deque_model_.iterator)));
}

void MainWindow::on_pb_edit_clicked() {
    if (deque_model_.items.begin() <= deque_model_.iterator && deque_model_.iterator < deque_model_.items.end()) {
        (*(deque_model_.iterator)) = ui->txt_elem_content->text().toStdString();
        ApplyModel();
    }
}
template<class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1, const std::deque<T>& half2, const Comp& comparator) {
    std::deque<T> result;
    auto it1 = half1.begin();
    auto it2 = half2.begin();
    
    while (it1 != half1.end() && it2 != half2.end()) {
        if (comparator(*it1, *it2)) {
            result.push_back(*it1);
            ++it1;
        } else {
            result.push_back(*it2);
            ++it2;
        }
    }
    
    result.insert(result.end(), it1, half1.end());
    result.insert(result.end(), it2, half2.end());
    
    return result;
}

template<class T, class Comp>
std::deque<T> MergeSort(const std::deque<T>& src, const Comp& comparator) {
    if (src.size() <= 1) {
        return src;
    }
    
    auto middle = src.begin() + src.size() / 2;
    std::deque<T> half1(src.begin(), middle);
    std::deque<T> half2(middle, src.end());
    
    return Merge(MergeSort(half1, comparator), MergeSort(half2, comparator), comparator);
}
void MainWindow::on_pb_pop_back_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_back();
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_push_back_clicked() {
    std::string new_item = ui->txt_elem_content->text().toStdString();
    deque_model_.items.push_back(new_item);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_erase_clicked() {
    deque_model_.items.erase(deque_model_.iterator);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_insert_clicked() {
    deque_model_.items.insert(deque_model_.iterator, ui->txt_elem_content->text().toStdString());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_clear_clicked() {
    deque_model_.items.clear();
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_iterator_dec_clicked() {
    if (deque_model_.items.begin() < deque_model_.iterator) {
        deque_model_.iterator -= 1;
        ApplyIterator();
    }
}

void MainWindow::on_pb_iterator_inc_clicked() {
    if (deque_model_.items.end() > deque_model_.iterator) {
        deque_model_.iterator += 1;
        ApplyIterator();
    }
}

void MainWindow::on_pb_iterator_begin_clicked() {
    deque_model_.iterator = deque_model_.items.begin();
    ApplyIterator();
}

void MainWindow::on_pb_iterator_end_clicked() {
    deque_model_.iterator = deque_model_.items.end();
    ApplyIterator();
}

void MainWindow::on_pb_tea_clicked() {
    static std::deque<std::string> tea {
        "Чай Лунцзин", "Эрл Грей", "Сенча", "Пуэр", "Дарджилинг",
        "Ассам", "Матча", "Ганпаудер", "Оолонг", "Лапсанг Сушонг"
    };
    deque_model_.items = tea;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_cakes_clicked() {
    static std::deque<std::string> cakes {
        "Красный бархат", "Наполеон", "Медовик", "Тирамису", "Прага",
        "Чизкейк", "Захер", "Эстерхази", "Морковный торт", "Чёрный лес"
    };
    deque_model_.items = cakes;
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_list_widget_currentRowChanged(int currentRow) {
    deque_model_.iterator = deque_model_.items.begin();

    if (0 <= currentRow && currentRow <= (int)(deque_model_.items.size())) {
        deque_model_.iterator += currentRow;
        ApplyIterator();
    }
}

void MainWindow::on_pb_resize_clicked() {
    bool ok;
    int new_size = ui->txt_size->text().toInt(&ok);
    if (!ok || new_size < 0) {
        return;
    }

    size_t current_pos = 0;
    if (deque_model_.iterator != deque_model_.items.end()) {
        current_pos = std::distance(deque_model_.items.begin(), deque_model_.iterator);
    }

    size_t old_size = deque_model_.items.size();
    deque_model_.items.resize(new_size);

    if (new_size > 0) {
        if (current_pos < static_cast<size_t>(new_size)) {
            deque_model_.iterator = deque_model_.items.begin() + current_pos;
        } else {
            deque_model_.iterator = deque_model_.items.end();
        }
    } else {
        deque_model_.iterator = deque_model_.items.end();
    }

    ApplyModel();
}

void MainWindow::on_pb_find_clicked() {
    std::string fstr = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::find(deque_model_.items.begin(), deque_model_.items.end(), fstr);
    ApplyIterator();
}

void MainWindow::on_pb_count_clicked() {
    std::string fstr = ui->le_count->text().toStdString();
    int count = std::count(deque_model_.items.begin(), deque_model_.items.end(), fstr);
    ui->lbl_count->setText(QString::number(count));
}

void MainWindow::on_pb_min_element_clicked() {
    deque_model_.iterator = std::min_element(deque_model_.items.begin(), deque_model_.items.end());
    ApplyIterator();
}

void MainWindow::on_pb_max_element_clicked() {
    deque_model_.iterator = std::max_element(deque_model_.items.begin(), deque_model_.items.end());
    ApplyIterator();
}

void MainWindow::SetRandomGen(const std::mt19937& random_gen) {
    random_gen_ = random_gen;
}

void MainWindow::on_pb_merge_sort_clicked() {
    deque_model_.items = MergeSort(deque_model_.items, std::less<std::string>());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_merge_sOrT_clicked() {
    struct CaseInsensitiveComparator {
        bool operator()(const std::string& a, const std::string& b) const {
            std::string lower_a = a;
            std::string lower_b = b;
            std::transform(a.begin(), a.end(), lower_a.begin(), ::tolower);
            std::transform(b.begin(), b.end(), lower_b.begin(), ::tolower);
            return lower_a < lower_b;
        }
    };
    CaseInsensitiveComparator comparator;
    deque_model_.items = MergeSort(deque_model_.items, comparator);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

std::string MainWindow::ToLower(std::string s) {
    setlocale(LC_ALL, "Russian");
    std::string res = {s};
    for(size_t ix = 0; ix < res.size(); ++ix) {
        res[ix] = std::tolower(res[ix]);
    }
    return res;
}

void MainWindow::on_pb_shuffle_clicked() {
    std::shuffle(deque_model_.items.begin(), deque_model_.items.end(), random_gen_);
    ApplyModel();
}

void MainWindow::on_pb_unique_clicked() {
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }
    auto to_del = std::unique(deque_model_.items.begin(), deque_model_.items.end());
    deque_model_.items.erase(to_del, deque_model_.items.end());
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_reverse_clicked() {
    std::reverse(deque_model_.items.begin(), deque_model_.items.end());
    ApplyModel();
}

void MainWindow::on_pb_push_front_clicked() {
    std::string new_item = ui->txt_elem_content->text().toStdString();
    deque_model_.items.push_front(new_item);
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_pop_front_clicked() {
    if (!deque_model_.items.empty()) {
        deque_model_.items.pop_front();
    }
    deque_model_.iterator = deque_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_pb_lower_bound_clicked() {
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }
    std::string bound = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::lower_bound(deque_model_.items.begin(), deque_model_.items.end(), bound);
    ApplyIterator();
}

void MainWindow::on_pb_upper_bound_clicked() {
    if (!std::is_sorted(deque_model_.items.begin(), deque_model_.items.end())) {
        return;
    }
    std::string bound = ui->txt_elem_content->text().toStdString();
    deque_model_.iterator = std::upper_bound(deque_model_.items.begin(), deque_model_.items.end(), bound);
    ApplyIterator();
}
