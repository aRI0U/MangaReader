#include <QLabel>
#include <QVBoxLayout>

#include "mangaentry.h"
#include "mangacover.h"
#include "constants.h"


MangaEntry::MangaEntry(QDir path)
    : m_path(path)
{
    initEntry();
}

void MangaEntry::initEntry()
{
    QVBoxLayout *layout = new QVBoxLayout;

    // initialize cover
    QLabel *m_cover = new MangaCover(m_path);

    // initialize title
    QLabel *m_title = new QLabel;
    m_title->setText(m_path.dirName());
    m_title->setFont(constants::MangaTitleFont);

    layout->addWidget(m_cover);
    layout->addWidget(m_title);

    layout->setAlignment(m_cover, Qt::AlignHCenter);
    layout->setAlignment(m_title, Qt::AlignHCenter);

    setLayout(layout);
    setStyleSheet("background-color: white;");
    setCursor(Qt::PointingHandCursor);
}

void MangaEntry::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
        // Process left click on cover
        emit clicked(m_path);
    }
}





