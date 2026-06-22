#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

struct Book {
    string id, title, author;
    int total, available;
};

struct Member {
    string id, name, email;
    vector<string> borrowed; // book IDs currently out
};

struct Record {
    int id;
    string member_id, book_id;
    string issued, due, returned; // returned is empty until they bring it back
};

map<string, Book>   books;
map<string, Member> members;
vector<Record>      records;

// ── helpers ──────────────────────────────────────────────────────────────────

string today() {
    time_t t = time(nullptr);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return buf;
}

string due_date() {
    time_t t = time(nullptr) + 14 * 86400;
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&t));
    return buf;
}

// positive = b is later than a
int days_apart(const string& a, const string& b) {
    auto parse = [](const string& s) {
        tm t = {};
        sscanf(s.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
        t.tm_year -= 1900; t.tm_mon -= 1;
        return mktime(&t);
    };
    return (int)difftime(parse(b), parse(a)) / 86400;
}

string lower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string input(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

// book ID is just title+author lowercased — good enough for a library system
string book_key(const string& title, const string& author) {
    string k = lower(title) + "|" + lower(author);
    replace(k.begin(), k.end(), ' ', '_');
    return k;
}

// ── persistence ───────────────────────────────────────────────────────────────
// plain pipe-delimited files, nothing fancy

void save() {
    {
        ofstream f("books.dat");
        for (auto& [id, b] : books)
            f << b.id << "|" << b.title << "|" << b.author
              << "|" << b.total << "|" << b.available << "\n";
    }
    {
        ofstream f("members.dat");
        for (auto& [id, m] : members) {
            f << m.id << "|" << m.name << "|" << m.email << "|";
            for (int i = 0; i < (int)m.borrowed.size(); i++) {
                if (i) f << ",";
                f << m.borrowed[i];
            }
            f << "\n";
        }
    }
    {
        ofstream f("records.dat");
        for (auto& r : records)
            f << r.id << "|" << r.member_id << "|" << r.book_id
              << "|" << r.issued << "|" << r.due << "|" << r.returned << "\n";
    }
}

void load() {
    string line;

    ifstream fb("books.dat");
    while (getline(fb, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        Book b;
        string t, a;
        getline(ss, b.id,     '|');
        getline(ss, b.title,  '|');
        getline(ss, b.author, '|');
        getline(ss, t, '|'); b.total     = stoi(t);
        getline(ss, a, '|'); b.available = stoi(a);
        books[b.id] = b;
    }

    ifstream fm("members.dat");
    while (getline(fm, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        Member m;
        string blist;
        getline(ss, m.id,    '|');
        getline(ss, m.name,  '|');
        getline(ss, m.email, '|');
        getline(ss, blist,   '|');
        if (!blist.empty()) {
            istringstream bs(blist);
            string bid;
            while (getline(bs, bid, ','))
                if (!bid.empty()) m.borrowed.push_back(bid);
        }
        members[m.id] = m;
    }

    ifstream fr("records.dat");
    while (getline(fr, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        Record r;
        string rid;
        getline(ss, rid,          '|'); r.id        = stoi(rid);
        getline(ss, r.member_id,  '|');
        getline(ss, r.book_id,    '|');
        getline(ss, r.issued,     '|');
        getline(ss, r.due,        '|');
        getline(ss, r.returned,   '|');
        records.push_back(r);
    }
}

// ── book functions ────────────────────────────────────────────────────────────

void add_book() {
    cout << "\nAdd Book\n";
    string title  = input("  Title  : ");
    string author = input("  Author : ");
    string n      = input("  Copies (Enter = 1): ");
    int copies = n.empty() ? 1 : stoi(n);

    string id = book_key(title, author);
    if (books.count(id)) {
        books[id].total     += copies;
        books[id].available += copies;
        cout << "  Already exists — updated to " << books[id].total << " total copies.\n";
    } else {
        books[id] = {id, title, author, copies, copies};
        cout << "  Added.\n";
    }
    save();
}

void list_books() {
    if (books.empty()) { cout << "\n  No books yet.\n"; return; }
    cout << "\n"
         << "  " << left << setw(4)  << "#"
                          << setw(34) << "Title"
                          << setw(24) << "Author"
                          << right << setw(7) << "Avail"
                          << setw(6)  << "Total" << "\n"
         << "  " << string(75, '-') << "\n";
    int i = 1;
    for (auto& [id, b] : books)
        cout << "  " << left << setw(4) << i++
             << setw(34) << b.title
             << setw(24) << b.author
             << right << setw(7) << b.available
             << setw(6) << b.total
             << (b.available ? "" : "  [out]") << "\n";
}

// ── member functions ──────────────────────────────────────────────────────────

void add_member() {
    cout << "\nAdd Member\n";
    string name  = input("  Name  : ");
    string email = input("  Email : ");

    // just count up from 1
    int n = members.size() + 1;
    string id = "M" + string(4 - to_string(n).size(), '0') + to_string(n);
    members[id] = {id, name, email, {}};
    save();
    cout << "  Registered. Member ID: " << id << "\n";
}

void list_members() {
    if (members.empty()) { cout << "\n  No members yet.\n"; return; }
    cout << "\n"
         << "  " << left << setw(8) << "ID"
                          << setw(26) << "Name"
                          << setw(32) << "Email"
                          << "Books out\n"
         << "  " << string(75, '-') << "\n";
    for (auto& [id, m] : members)
        cout << "  " << left << setw(8) << m.id
             << setw(26) << m.name
             << setw(32) << m.email
             << m.borrowed.size() << "\n";
}

// ── selection helpers ─────────────────────────────────────────────────────────

Book* pick_book(bool only_available = false) {
    vector<Book*> list;
    for (auto& [id, b] : books)
        if (!only_available || b.available > 0)
            list.push_back(&b);
    if (list.empty()) { cout << "  No books" << (only_available ? " available" : "") << ".\n"; return nullptr; }

    for (int i = 0; i < (int)list.size(); i++)
        cout << "  " << i+1 << ". " << list[i]->title << " by " << list[i]->author
             << " (" << list[i]->available << "/" << list[i]->total << ")\n";

    string s = input("  Choice: ");
    int n = s.empty() ? 0 : stoi(s);
    if (n < 1 || n > (int)list.size()) { cout << "  Invalid.\n"; return nullptr; }
    return list[n-1];
}

Member* pick_member() {
    vector<Member*> list;
    for (auto& [id, m] : members) list.push_back(&m);
    if (list.empty()) { cout << "  No members registered.\n"; return nullptr; }

    for (int i = 0; i < (int)list.size(); i++)
        cout << "  " << i+1 << ". [" << list[i]->id << "] " << list[i]->name << "\n";

    string s = input("  Choice: ");
    int n = s.empty() ? 0 : stoi(s);
    if (n < 1 || n > (int)list.size()) { cout << "  Invalid.\n"; return nullptr; }
    return list[n-1];
}

// ── issue / return ────────────────────────────────────────────────────────────

void issue_book() {
    cout << "\nIssue Book\n";
    cout << "  Pick a book:\n";
    Book* book = pick_book(true);
    if (!book) return;

    cout << "  Pick a member:\n";
    Member* mem = pick_member();
    if (!mem) return;

    // don't let someone borrow the same book twice
    for (auto& r : records)
        if (r.member_id == mem->id && r.book_id == book->id && r.returned.empty()) {
            cout << "  They already have this book.\n";
            return;
        }

    Record r;
    r.id        = records.size() + 1;
    r.member_id = mem->id;
    r.book_id   = book->id;
    r.issued    = today();
    r.due       = due_date();
    records.push_back(r);

    book->available--;
    mem->borrowed.push_back(book->id);
    save();

    cout << "  Issued. Due back by " << r.due << ".\n";
}

void return_book() {
    cout << "\nReturn Book\n";

    vector<Member*> active;
    for (auto& [id, m] : members)
        if (!m.borrowed.empty()) active.push_back(&m);
    if (active.empty()) { cout << "  No books are currently checked out.\n"; return; }

    for (int i = 0; i < (int)active.size(); i++)
        cout << "  " << i+1 << ". " << active[i]->name
             << " (" << active[i]->borrowed.size() << " out)\n";

    string s = input("  Pick member: ");
    int n = s.empty() ? 0 : stoi(s);
    if (n < 1 || n > (int)active.size()) { cout << "  Invalid.\n"; return; }
    Member* mem = active[n-1];

    vector<Record*> out;
    for (auto& r : records)
        if (r.member_id == mem->id && r.returned.empty())
            out.push_back(&r);

    for (int i = 0; i < (int)out.size(); i++) {
        auto& b = books[out[i]->book_id];
        cout << "  " << i+1 << ". " << b.title << " (due " << out[i]->due << ")\n";
    }

    string s2 = input("  Pick book: ");
    int n2 = s2.empty() ? 0 : stoi(s2);
    if (n2 < 1 || n2 > (int)out.size()) { cout << "  Invalid.\n"; return; }

    Record* rec = out[n2-1];
    rec->returned = today();
    books[rec->book_id].available++;

    auto& bv = mem->borrowed;
    bv.erase(find(bv.begin(), bv.end(), rec->book_id));

    int late = days_apart(rec->due, today());
    if (late > 0)
        cout << "  Returned " << late << " day(s) late.\n";
    else
        cout << "  Returned on time.\n";

    save();
}

// ── search ────────────────────────────────────────────────────────────────────

void search() {
    string q = lower(input("\nSearch (title or author): "));
    if (q.empty()) return;

    bool found = false;
    for (auto& [id, b] : books) {
        if (lower(b.title).find(q) != string::npos || lower(b.author).find(q) != string::npos) {
            cout << "  " << b.title << " by " << b.author
                 << "  [" << b.available << "/" << b.total << " available]\n";
            found = true;
        }
    }
    if (!found) cout << "  Nothing found.\n";
}

// ── history ───────────────────────────────────────────────────────────────────

void history() {
    if (records.empty()) { cout << "\n  No records yet.\n"; return; }
    cout << "\n"
         << "  " << left << setw(5)  << "#"
                          << setw(18) << "Member"
                          << setw(28) << "Book"
                          << setw(12) << "Issued"
                          << setw(12) << "Due"
                          << "Returned\n"
         << "  " << string(85, '-') << "\n";
    for (auto& r : records) {
        string mname  = members.count(r.member_id) ? members[r.member_id].name : r.member_id;
        string btitle = books.count(r.book_id)     ? books[r.book_id].title    : r.book_id;
        cout << "  " << left << setw(5)  << r.id
             << setw(18) << mname
             << setw(28) << btitle
             << setw(12) << r.issued
             << setw(12) << r.due
             << (r.returned.empty() ? "-" : r.returned) << "\n";
    }
}

// ── main ──────────────────────────────────────────────────────────────────────

int main() {
    load();

    while (true) {
        cout << "\n=== Library Management System ===\n"
             << "  1. Add book        5. Issue book\n"
             << "  2. List books      6. Return book\n"
             << "  3. Add member      7. Search\n"
             << "  4. List members    8. History\n"
             << "  0. Exit\n";

        string choice = input("> ");

        if      (choice == "1") add_book();
        else if (choice == "2") list_books();
        else if (choice == "3") add_member();
        else if (choice == "4") list_members();
        else if (choice == "5") issue_book();
        else if (choice == "6") return_book();
        else if (choice == "7") search();
        else if (choice == "8") history();
        else if (choice == "0") break;
        else cout << "  Try again.\n";
    }

    return 0;
}