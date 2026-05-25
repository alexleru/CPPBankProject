# Промпт для Opus: подготовка `CPPBankProject` к демонстрации SCC

---

## Системный контекст для агента

```
Это маленький учебный C++03/11 проект «банк».

Твоя задача — расширить проект так, чтобы его граф зависимостей содержал
три циклических Strongly Connected Components разного размера (2, 5 и 8–10).
Эти SCC будут использоваться для демонстрации работы плагина
java_cpp_chunkagent (конвертация C++ → Java Spring). Проект должен
после расширения компилироваться `make all` без ошибок и предупреждений
о неиспользуемых сущностях.
```

---

## Жёсткие требования

### Что сохраняется неизменным

1. Файлы которые не указаны ниже

### Что должно появиться

Три SCC, явно вписанные в граф `#include` (заголовки) **и** взаимных вызовов методов
(тела в .cpp). Граф зависимостей должен выглядеть так после генерации:

#### SCC A — размер 2 (bidirectional ownership)

```
Account.h        ↔  Transaction.h
Account.cpp      uses Transaction
Transaction.cpp  uses Account
```

- `Account` хранит `std::vector<Transaction*> history`.
- `Transaction` хранит указатели `Account* source` и `Account* dest`.
- `Transaction::apply()` вызывает `source->debit()` и `dest->credit()`.
- `Account::addTransaction(Transaction*)` добавляет в `history` и записывает
  `t->setSequence(history.size())`.
- Forward declaration `class Transaction;` в `Account.h`,
  forward declaration `class Account;` в `Transaction.h` —
  чтобы оба заголовка не пытались `#include` друг друга напрямую,
  а в .cpp оба `#include` обоих заголовков.
- Это создаёт цикл в графе зависимостей чанкера (он анализирует .cpp + .h
  и видит взаимные использования типов).

#### SCC B — размер 5 (Visitor pattern с двойной диспетчеризацией)

```
TransactionVisitor.h  ↔  Deposit.h
TransactionVisitor.h  ↔  Withdrawal.h
TransactionVisitor.h  ↔  Transfer.h
TransactionVisitor.h  ↔  LoanPayment.h
```

- `TransactionVisitor` — абстрактный базовый класс с чисто-виртуальными
  методами:
  ```cpp
  virtual void visit(Deposit*      d) = 0;
  virtual void visit(Withdrawal*   w) = 0;
  virtual void visit(Transfer*     t) = 0;
  virtual void visit(LoanPayment*  l) = 0;
  ```
- Каждый из `Deposit`, `Withdrawal`, `Transfer`, `LoanPayment` наследуется
  от `Transaction` (из SCC A) и добавляет `virtual void accept(TransactionVisitor& v)`,
  тело: `v.visit(this);`.
- Конкретный visitor `LoggingVisitor : public TransactionVisitor` — пишет
  каждую операцию в `std::ostream` (для main.cpp).
- Forward declarations: в `TransactionVisitor.h` объявлены
  `class Deposit; class Withdrawal; class Transfer; class LoanPayment;`,
  в каждом из заголовков транзакций — `class TransactionVisitor;`.
- В .cpp всех пяти классов есть взаимные `#include`.

ВАЖНО: `Deposit`, `Withdrawal`, `Transfer`, `LoanPayment` также наследуются от
`Transaction` (из SCC A) — это создаёт **архитектурную связь** между SCC A и SCC B,
но не сливает их в один SCC, потому что наследование однонаправленное.

#### SCC C — размер 8–10 (Observer + Mediator mesh)

```
Bank ↔ Customer ↔ Account ↔ Loan ↔ AuditLogger ↔ NotificationCenter
                                              ↔ BranchManager  ↔ RiskAnalyzer
```

Конкретный набор взаимных связей:

- `Bank` хранит `std::vector<Customer*>`, `std::vector<BranchManager*>`,
  `AuditLogger*`, `NotificationCenter*`.
- `Customer` хранит `std::vector<Account*>`, `std::vector<Loan*>`, `Bank* owner`.
- `Account` хранит `Customer* holder` (обратная ссылка), `Bank* bank`,
  `AuditLogger* logger`.
- `Loan` хранит `Customer* borrower`, `Account* repaymentAccount`,
  `RiskAnalyzer* analyzer`.
- `AuditLogger` принимает callbacks: `void onAccountCreated(Account*)`,
  `void onLoanApproved(Loan*)`, `void onCustomerRegistered(Customer*)`,
  `void notify(NotificationCenter*, const std::string&)`.
- `NotificationCenter` хранит `std::vector<Customer*> subscribers`,
  `AuditLogger* auditLog`. Метод `broadcast(Bank*, const std::string&)`.
- `BranchManager` хранит `Bank* bank`, `std::vector<Customer*> assigned`,
  `RiskAnalyzer* analyzer`. Метод `escalate(Loan*, AuditLogger*)`.
- `RiskAnalyzer` хранит `Bank* bank`, `AuditLogger* logger`. Метод
  `assess(Loan*) const`, `flagCustomer(Customer*, BranchManager*)`.

Каждая пара выше — реальное взаимное использование типов (поля + вызовы методов).
Forward declarations расставить аккуратно, чтобы заголовки могли быть
включены друг другом без бесконечной рекурсии (циклы заголовков ломаются
через `class X;`, тела вызовов — в .cpp).

ОБЯЗАТЕЛЬНО: существующий `Bank` и `Customer` (которые сейчас в include/) —
**переименовать в `LegacyBank` и `LegacyCustomer`** и оставить как есть
(они станут acyclic-узлами, контрольной группой). Новые `Bank` и `Customer`,
участвующие в SCC C, написать заново как новые файлы.

#### SCC D — размер 5 (Tier-B: Reporting pipeline)

**Цель**: изолированный «чистый» SCC размером 5 — для демонстрации работы
алгоритмического планировщика (Tier-B) рядом с мега-SCC (Tier-C/D).
**Важно**: ни один из пяти классов SCC D **не должен ссылаться** на классы из
SCC A/B/C (Account, Transaction, Bank, Customer, Loan и т. д.) —
полная изоляция обязательна для получения отдельного SCC в графе.

```
ReportEngine ↔ ReportFilter ↔ ReportSection ↔ ReportFormatter ↔ ReportWriter
     ↑_______________________________________________________________|
```

Конкретный набор взаимных связей:

- `ReportEngine` хранит `ReportFilter* filter`, `ReportWriter* writer`.
  Методы: `generate(std::ostream&)` — вызывает `filter->apply(this)`,
  затем `writer->write(this, stream)`;
  `void onPageReady(ReportSection* s)` — callback, вызываемый `ReportWriter`'ом.
  `std::string getDateRange() const` — используется `ReportFilter`'ом.

- `ReportFilter` хранит `ReportEngine* engine`.
  Методы: `void apply(ReportEngine* e)` — вызывает `e->getDateRange()`,
  строит набор правил;
  `bool getActiveRules(ReportSection* s) const` — используется `ReportSection`'ом,
  проверяет соответствие секции текущим правилам.

- `ReportSection` хранит `ReportFilter* filter`, `ReportFormatter* formatter`.
  Методы: `void render(std::ostream& out)` — вызывает `filter->getActiveRules(this)`,
  затем `formatter->format(this)`;
  `std::string getTitle() const` — используется `ReportFormatter`'ом.

- `ReportFormatter` хранит `ReportSection* current`.
  Методы: `void format(ReportSection* s)` — обновляет `current`, вызывает
  `current->getTitle()`, форматирует заголовок и тело;
  `std::string getStyle() const` — используется `ReportWriter`'ом.

- `ReportWriter` хранит `ReportEngine* engine`, `ReportFormatter* formatter`.
  Методы: `void write(ReportEngine* e, std::ostream& out)` — создаёт
  `ReportSection`, вызывает `section->render(out)`, `formatter->getStyle()`,
  затем `e->onPageReady(section)` — замыкает цикл обратно на `ReportEngine`.

Forward declarations в каждом заголовке (`class X;`), полные `#include` в .cpp.
Этот паттерн (pipeline с обратным callback'ом) типичен для legacy report-генераторов
и даёт детерминированный SCC размером ровно 5 без зависимости от остальных классов.

### Файловая структура после изменений

```
include/
  Constants.h           (расширить: добавить typedef AccountId, LoanId, Money)
  Enums.h               (расширить: добавить TransactionType, LoanStatus)
  Globals.h             (без изменений или с минимальным расширением счётчиков)
  Utils.h               (без изменений)
  AgeVerifier.h         (НЕ трогать)

  LegacyBank.h          (бывший Bank.h, переименован)
  LegacyCustomer.h      (бывший Customer.h, переименован)

  Account.h             (SCC A)
  Transaction.h         (SCC A, базовый класс)

  TransactionVisitor.h  (SCC B)
  Deposit.h             (SCC B)
  Withdrawal.h          (SCC B)
  Transfer.h            (SCC B)
  LoanPayment.h         (SCC B)
  LoggingVisitor.h      (concrete visitor, использует SCC B)

  Bank.h                (SCC C)
  Customer.h            (SCC C)
  Loan.h                (SCC C)
  AuditLogger.h         (SCC C)
  NotificationCenter.h  (SCC C)
  BranchManager.h       (SCC C)
  RiskAnalyzer.h        (SCC C)

  ReportEngine.h        (SCC D)
  ReportFilter.h        (SCC D)
  ReportSection.h       (SCC D)
  ReportFormatter.h     (SCC D)
  ReportWriter.h        (SCC D)

src/
  Globals.cpp, Utils.cpp, AgeVerifier.cpp  (как есть)
  LegacyBank.cpp, LegacyCustomer.cpp        (переименованные старые)

  Account.cpp, Transaction.cpp              (SCC A)
  TransactionVisitor.cpp                    (если нужны не-inline тела)
  Deposit.cpp, Withdrawal.cpp, Transfer.cpp, LoanPayment.cpp  (SCC B)
  LoggingVisitor.cpp                        (concrete)

  Bank.cpp, Customer.cpp, Loan.cpp,
  AuditLogger.cpp, NotificationCenter.cpp,
  BranchManager.cpp, RiskAnalyzer.cpp        (SCC C)

  ReportEngine.cpp, ReportFilter.cpp,
  ReportSection.cpp, ReportFormatter.cpp,
  ReportWriter.cpp                           (SCC D)

  main.cpp                                  (расширить демо-сценариями)
```

### main.cpp

Расширить меню до пунктов, демонстрирующих новые SCC:
1. Existing menu items (Legacy bank create/list, function-pointer demo, age verify) — сохранить через `LegacyBank`/`LegacyCustomer`.
2. Новый сценарий «New bank flow»: создаёт `Bank`, `Customer`, `Account`,
   делает несколько `Deposit`/`Withdrawal`, апплаит их через `LoggingVisitor`,
   выдаёт `Loan` через `BranchManager`, оценивает через `RiskAnalyzer`,
   рассылает уведомления через `NotificationCenter`, логирует в `AuditLogger`.
3. Новый сценарий «Generate report»: создаёт `ReportEngine`, `ReportFilter`,
   `ReportSection`, `ReportFormatter`, `ReportWriter`; запускает
   `engine->generate(std::cout)` — pipeline должен пройти полный цикл
   через все 5 классов SCC D и вывести форматированный отчёт.
   **Ни один объект SCC D не должен принимать параметры типов из SCC A/B/C.**
4. Выход.

Каждый сценарий должен делать осмысленный data-flow, не просто заглушки —
каждый класс из SCC C и SCC D должен вызвать минимум 2 метода другого класса
из того же SCC.

### Makefile

Обновить:
- список объектных файлов;
- зависимости (можно через `wildcard $(SRCDIR)/*.cpp`);
- сохранить существующие targets `all`, `clean`, `run`, native-lib обвязку.

После `make all` бинарь должен собираться без warning’ов на gcc/clang
с флагами `-Wall -Wextra -std=c++11`.

### Документация

Создать `docs/SCC_DEMO_LAYOUT.md` со следующей структурой:

```markdown
# SCC structure of the demo project

## Overview
4 cyclic SCC разных размеров и tier'ов:
- SCC A+C (мега-SCC, размер ~10–12): Account + Transaction + Bank + Customer +
  Loan + AuditLogger + NotificationCenter + BranchManager + RiskAnalyzer —
  демонстрирует Tier-C/D (worst-case, mediator/observer-mesh)
- SCC B (размер 5): TransactionVisitor + Deposit + Withdrawal + Transfer +
  LoanPayment — демонстрирует Tier-B (Visitor-pattern цикл)
- SCC D (размер 5): ReportEngine + ReportFilter + ReportSection +
  ReportFormatter + ReportWriter — демонстрирует Tier-B (pipeline-цикл,
  полностью изолирован от банковских классов)
Plus 7 acyclic legacy/utility classes
(LegacyBank, LegacyCustomer, Utils, Globals, Constants, Enums, AgeVerifier).

## SCC A+C (мега-SCC): Account/Transaction + Observer/Mediator mesh
[список членов, тип каждого ребра, рисунок ASCII]
Примечание: SCC A (Account ↔ Transaction) и SCC C (Bank-mesh) объединяются
в один мега-SCC через обратные указатели Account.holder→Customer и
Account.bank→Bank. Это намеренный worst-case для демонстрации.

## SCC B (size 5): Visitor pattern
[…]

## SCC D (size 5): Reporting pipeline
[…]

## Acyclic baseline classes
[…]

## Expected dependency_graph.json
DependencyGraphService.get_cyclic_sccs() должен вернуть ровно 3 SCC:
- 1 мега-SCC размером ~10–12 (A+C merged)
- 1 SCC размером 5 (Visitor, SCC B)
- 1 SCC размером 5 (Reporting pipeline, SCC D)
```

Этот документ — для команды, оценивающей демо. Он же используется как
ground truth при валидации работы плагина.

---

## Что НЕ нужно делать

1. **Не вводить C++17/20 features** (`std::optional`, structured bindings, `auto` для
   полей, `std::variant`, `<filesystem>`). Стиль остаётся C++03/11 с typedef’ами.
2. **Не использовать smart pointers** (`std::unique_ptr`, `std::shared_ptr`).
   Все владения через сырые указатели + явное `delete` в деструкторах.
3. **Не вводить namespace** для новых классов — они должны быть в глобальном
   namespace, как и существующие.
4. **Не пытаться сделать SCC искусственно «правильными»** — наоборот,
   нужны те самые проблемные паттерны, которые встречаются в legacy
   (взаимное владение, обратные указатели, mediator-mesh).
5. **Не оптимизировать архитектуру** — не выносить интерфейсы для разрыва циклов,
   не вводить event bus. Циклы должны остаться в графе.
6. **Не добавлять unit-тесты** — `run_tests.py` для smoke-проверки сборки достаточно.
7. **Не трогать native age_verifier** и его обвязку.

---

## Критерии приёмки

Запускающий проверяет:

1. `make clean && make all` → собирается без ошибок и warning’ов.
2. `./bank` (или эквивалент) запускается, меню работает, новый сценарий
   полностью отрабатывает и печатает понятный лог взаимодействия SCC C.
3. `git status` → все новые файлы добавлены; нет «забытых» untracked-файлов.
4. `docs/SCC_DEMO_LAYOUT.md` существует и описывает 3 итоговых SCC.
5. После запуска C++-чанкера плагина `java_cpp_chunkagent` на этом проекте
   `dependency_graph.json` содержит **ровно 3** циклических SCC:
   - 1 мега-SCC (~10–12 узлов: Account + Transaction + Bank + Customer и др.)
   - 1 SCC размером 5 (Visitor pattern: TransactionVisitor + 4 наследника)
   - 1 SCC размером 5 (Reporting: ReportEngine + 4 компонента)
   Критично: классы SCC D **не должны** попасть в мега-SCC (проверить
   отсутствием рёбер между ReportEngine/Filter/Section/Formatter/Writer
   и банковскими классами в `dependency_graph.json`).
6. Общий объём кода: 2000–3500 LOC (добавились 5 классов SCC D).
   Если меньше — добавить осмысленное поведение в методы (логирование,
   валидация), не заглушки. Если больше — упростить.

---

## Порядок работы агента

1. Создать ветку `scc-demo` от `main` (или текущей рабочей).
2. Переименовать `Bank.h`/`Bank.cpp` → `LegacyBank.h`/`LegacyBank.cpp`,
   `Customer.h`/`Customer.cpp` → `LegacyCustomer.h`/`LegacyCustomer.cpp`.
   Обновить `include`-директивы внутри них и в `main.cpp`.
3. Реализовать SCC A (Account + Transaction).
4. Реализовать SCC B (TransactionVisitor + 4 наследника + LoggingVisitor).
5. Реализовать SCC C (7–8 классов с mediator/observer-mesh).
5a. Реализовать SCC D (ReportEngine + ReportFilter + ReportSection +
    ReportFormatter + ReportWriter). Убедиться, что ни один заголовок SCC D
    не включает и не forward-декларирует классы из SCC A/B/C.
6. Расширить `Constants.h`, `Enums.h` нужными typedef’ами.
7. Переписать `main.cpp` под расширенное меню.
8. Обновить `Makefile`.
9. Написать `docs/SCC_DEMO_LAYOUT.md`.
10. `make clean && make all` — итерировать до полной сборки без warning’ов.
11. `make run` (или ручной запуск) — проверить, что меню работает,
    новый сценарий проходит без ошибок памяти и Segfault’ов.
12. Закоммитить изменения серией атомарных коммитов:
    - `chore: rename existing Bank/Customer to Legacy*`
    - `feat: add SCC A — Account ↔ Transaction`
    - `feat: add SCC B — Visitor pattern (5 classes)`
    - `feat: add SCC C — Observer/Mediator mesh (8 classes)`
    - `feat: extend main.cpp with new bank flow demo`
    - `docs: describe SCC layout for conversion demo`
13. **Не пушить ветку** автоматически — оставить для ревью человеком.

---

## Подсказки по реализации сложных мест

### Forward declarations
В заголовке `Account.h`:
```cpp
#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <vector>
class Transaction;  // forward — полное определение в Transaction.h
class Customer;     // forward — для SCC C ссылки
class Bank;
class AuditLogger;

class Account {
    // поля используют только указатели на forward-объявленные типы
    std::vector<Transaction*> history;
    Customer* holder;
    Bank* bank;
    AuditLogger* logger;
public:
    // объявления методов, тела — в .cpp
    void addTransaction(Transaction* t);
    void debit(double amount);
    void credit(double amount);
    // ...
};
#endif
```

В `Account.cpp` уже идёт полный `#include "Transaction.h"`, `#include "Customer.h"`
и т. д.

### Корректность владения
Кто `delete`-ит что — задокументировать кратким комментарием в `.h`.
Простейший вариант: `Bank` владеет `Customer*`, `AuditLogger*`,
`NotificationCenter*`, `BranchManager*`, `RiskAnalyzer*`; `Customer` владеет
своими `Account*` и `Loan*`; `Account` владеет своими `Transaction*`.
Обратные указатели — **не-владеющие** (raw observation pointers).

### Деструкторы
Каждый владеющий класс — пройти `delete` по своим контейнерам в деструкторе.
Не забыть `virtual ~Transaction()` (раз есть наследники).

### main.cpp поток
```
initializeGlobals(); 
Bank* bank = new Bank("Demo Bank");
AuditLogger* logger = new AuditLogger();
NotificationCenter* nc = new NotificationCenter(logger);
bank->setAuditLogger(logger);
bank->setNotificationCenter(nc);

BranchManager* mgr = new BranchManager(bank, new RiskAnalyzer(bank, logger));
bank->addBranchManager(mgr);

Customer* alice = new Customer("Alice", "Smith", bank);
bank->registerCustomer(alice);

Account* acc = new Account(alice, bank, logger);
alice->addAccount(acc);

Deposit* d = new Deposit(acc, 1000.0);
acc->addTransaction(d);
d->apply();

LoggingVisitor v(std::cout);
d->accept(v);

Loan* loan = new Loan(alice, acc, mgr->getRiskAnalyzer());
mgr->approveLoan(loan, logger);
nc->broadcast(bank, "Loan approved for Alice");

delete bank;   // каскадный delete через деструкторы
```

(детали могут отличаться, главное — поток данных через все 3 SCC).

---

## Итоговый отчёт агента

После выполнения агент должен сообщить:

- Список добавленных/изменённых файлов с краткой ролью каждого.
- Размер каждой SCC (по числу классов).
- Итоговое количество LOC.
- Результат `make all` (success/количество предупреждений).
- Готовый branch name и список коммитов.
- Любые отступления от спецификации с обоснованием.
