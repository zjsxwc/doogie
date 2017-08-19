#include <QtTest>
#include <QtWidgets>

#include "blocker_rules.h"

namespace doogie {

class BlockerRulesTest : public QObject {
  Q_OBJECT

 private:
  // Auto deleted at end of each test case
  BlockerRules* ParsedRules(const QString& text, int file_index = 1) {
    cleanupLastRules();
    QTextStream stream(&QString(text));
    last_rules_ = new BlockerRules;
    last_rules_->AddRules(&stream, file_index);
    return last_rules_;
  }

  void cleanupLastRules() {
    if (last_rules_) {
      delete last_rules_;
      last_rules_ = nullptr;
    }
  }

  BlockerRules* last_rules_ = nullptr;

 private slots:  // NOLINT(whitespace/indent)
  void cleanup() {
    cleanupLastRules();
  }

  void testSimpleStaticRules() {
    auto rules = ParsedRules(
        "&adbannerid=\n"
        "/ads/profile/*\n"
        "/advert-$domain=~advert-technology.com|~advert-technology.ru\n"
        "||7pud.com^$third-party\n"
        "@@||speedtest.net^*/results.php$xmlhttprequest",
        0);
    // qDebug().noquote() << "JSON:\n" <<
    //                       QJsonDocument(rules->RuleTree()).toJson();
    auto rule = rules->FindStaticRule(
          "http://example.com/?foo=bar&adbannerid=35",
          "http://example.com/",
          BlockerRules::StaticRule::AllRequests);
    QVERIFY(rule);
  }
};

}  // namespace doogie

QTEST_MAIN(doogie::BlockerRulesTest)
#include "blocker_rules.test.moc"
