#include <QLabel>
#include <QVBoxLayout>

#include "mangaentry.h"
#include "mangacover.h"
#include "constants.h"

MangaEntry::MangaEntry(QWidget *parent, QDir path)
    : QWidget(parent), m_path(path)
{
    initEntry();
}

MangaEntry::MangaEntry(QDir path)
    : m_path(path)
{
    initEntry();
}

void MangaEntry::initEntry()
{
    QVBoxLayout *layout = new QVBoxLayout;

    // initialize cover
    QLabel *m_cover = new QLabel;

    m_cover->setPixmap(MangaCover(m_path));

    // initialize title
    QLabel *m_title = new QLabel;
    m_title->setText(m_path.dirName());
    m_title->setFont(constants::MangaTitleFont);

    layout->addWidget(m_cover);
    layout->addWidget(m_title);

    layout->setAlignment(m_cover, Qt::AlignHCenter);
    layout->setAlignment(m_title, Qt::AlignHCenter);

    setLayout(layout);
}
