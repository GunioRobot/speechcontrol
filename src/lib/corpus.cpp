#include "corpus.hpp"
#include "sentence.hpp"

#include <QDebug>
#include <QDateTime>
#include <QDomDocument>

using SpeechControl::Corpus;
using SpeechControl::Sentence;
using SpeechControl::CorpusList;
using SpeechControl::SentenceList;

Corpus::Corpus(const QUuid& p_uuid) : QObject(), m_dom(new QDomDocument) {
    load(p_uuid);
}

/// @todo This list has to be ordered from '0' to 'n-1'.
/// @note Since this sorts the values by index, the value returned should be called once and stored into a variable, to prevent overhead.
/// @todo Cache the ordered listing and update it whenever it's modified.
SentenceList Corpus::sentences() const
{
    return m_sntncLst;
}

/// @todo Add the Sentence to the XML document and then to the list.
Sentence* Corpus::addSentence(Sentence *l_phrs)
{
    m_sntncLst << l_phrs;
    return l_phrs;
}

Sentence* Corpus::addSentence(const QString &l_txt, const QFile *l_audio)
{
    qDebug() << "Adding sentence" << l_txt << "...";
    Sentence* l_sentence = Sentence::create(this,l_txt);

    if (l_audio)
        l_sentence->m_elem->attribute(QUrl::fromLocalFile(l_audio->fileName()).toString());

    return l_sentence;
}

Corpus & Corpus::operator <<(Sentence *l_phrs)
{
    this->addSentence(l_phrs);
    return *this;
}

/// @todo Just invoke the above method.
Corpus & Corpus::operator <<(SentenceList &l_lst)
{
    foreach (Sentence* l_phrs, l_lst)
    this->addSentence(l_phrs);

    return *this;
}

/// @todo Build a dictionary from the said text.
/// @todo Find a way to keep the text in an ordinal fashion.
Corpus * Corpus::create(const QStringList& p_text)
{
    QUuid l_uuid = QUuid::createUuid();
    QDir l_dir;
    if (!l_dir.mkpath(getPath(l_uuid).toLocalFile())) {
        qWarning() << "Can't make corpus" << l_uuid;
        return 0;
    }

    QDomDocument l_dom("Corpus");
    QDomElement l_rootElem = l_dom.createElement("Corpuses");
    QDomElement l_dateElem = l_dom.createElement("Timing");
    QDomElement l_sentencesElem = l_dom.createElement("Sentences");

    l_rootElem.setAttribute("Uuid",l_uuid.toString());
    l_rootElem.setAttribute("Version",0.01);
    l_dateElem.setAttribute("Created",QDateTime::currentDateTimeUtc().toString());

    l_rootElem.appendChild(l_dateElem);
    l_rootElem.appendChild(l_sentencesElem);
    l_dom.appendChild(l_rootElem);

    const QUrl l_path = getPath(l_uuid);
    QFile* l_file = new QFile(l_path.toLocalFile() + "/corpus.xml");
    l_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream l_strm(l_file);
    l_dom.save(l_strm,4);

    Corpus* l_corpus = Corpus::obtain(l_uuid);
    Q_FOREACH(const QString& l_str, p_text) {
        Sentence* l_sent = l_corpus->addSentence(l_str.simplified().trimmed(),0);
        l_corpus->m_dom->documentElement().namedItem("Sentences").appendChild(*l_sent->m_elem);
    }

    l_corpus->save();
    return l_corpus;
}

const bool Corpus::exists(const QUuid& l_uuid) {
    return QFile::exists(getPath(l_uuid).toLocalFile());
}

QUrl Corpus::getPath(const QUuid &l_uuid)
{
    return QDir::homePath() + "/.speechcontrol/corpus/" + l_uuid.toString();
}

QUrl Corpus::audioPath() const
{
    return getPath(this->uuid()).toLocalFile() + "/audio";
}

const QUuid Corpus::uuid() const
{
    return m_dom->documentElement().attribute("Uuid");
}

Corpus* Corpus::obtain(const QUuid &l_uuid)
{
    const QString l_path = getPath(l_uuid).toLocalFile() + "/corpus.xml";

    if (!QFile::exists(l_path)) {
        qDebug() << "Corpus not found at" << l_path;
        return 0;
    }

    return new Corpus(l_uuid);
}

void Corpus::load(const QUuid &p_uuid)
{
    const QUrl l_path = getPath(p_uuid);
    QFile* l_file = new QFile(l_path.toLocalFile() + "/corpus.xml");

    if (l_file->exists() && l_file->open(QIODevice::ReadOnly)) {
        if (!m_dom)
            m_dom = new QDomDocument("Corpus");

        if (!m_dom->setContent(l_file)) {
            qDebug() << "Failed to load corpus.";
            return;
        }

        QDomNodeList l_elems = m_dom->documentElement().firstChildElement("Sentences").childNodes();

        for (int i = 0; i < l_elems.count(); ++i) {
            QDomElement l_elem = l_elems.at(i).toElement();
            qDebug() << "Loading sentence:" << l_elem.attribute("uuid");

            if (l_elem.isNull()) continue;

            Sentence* l_sntc = new Sentence(this,(new QDomElement(l_elems.at(i).toElement())));
            qDebug() << "Loaded sentence:" << l_sntc->text();
            addSentence(l_sntc);
        }
    } else
        qDebug() << "Failed to open corpus XML file.";
}

void Corpus::save()
{
    const QUrl l_path = getPath(this->uuid());
    QFile* l_file = new QFile(l_path.toLocalFile() + "/corpus.xml");
    if (l_file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream l_strm(l_file);
        m_dom->save(l_strm,4);
    } else
        qDebug() << "Can't open:" << l_file->errorString();
}

CorpusList Corpus::allCorpuses()
{
    CorpusList l_lst;
    QDir l_dir(QDir::homePath() + "/.speechcontrol/corpus/");
    l_dir.setFilter(QDir::Dirs);
    QStringList l_results = l_dir.entryList(QStringList() << "*");
    Q_FOREACH(const QString& l_uuid, l_results) {
        l_lst << Corpus::obtain(QUuid(l_uuid));
    }

    return l_lst;
}

/// @todo What to clean-up?
Corpus::~Corpus()
{

}
