#include <QLabel>
#include <QVBoxLayout>

#include "mangaentry.h"
#include "constants.h"

MangaEntry::MangaEntry(QWidget *parent, QString manga)
    : QWidget(parent), m_manga(manga)
{
    initEntry();
}

MangaEntry::MangaEntry(QString manga)
    : m_manga(manga)
{
    initEntry();
}

void MangaEntry::initEntry()
{
    QVBoxLayout *layout = new QVBoxLayout;

    // initialize cover
    QLabel *m_cover = new QLabel;
//    m_cover->setText("cover.png");
    m_cover->setPixmap(QPixmap("cover.png"));

    // initialize title
    QLabel *m_title = new QLabel;
    m_title->setText(m_manga);
    m_title->setFont(constants::MangaTitleFont);

    layout->addWidget(m_cover);
    layout->addWidget(m_title);

    layout->setAlignment(m_cover, Qt::AlignHCenter);
    layout->setAlignment(m_title, Qt::AlignHCenter);

    setLayout(layout);
}
