
#include "tablebottombox.hpp"

#include <sstream>

#include <QStatusBar>
#include <QStackedLayout>
#include <QLabel>

#include "creator.hpp"

void CSVWorld::TableBottomBox::updateStatus()
{
    if (mShowStatusBar)
    {
        static const char *sLabels[4] = { "record", "deleted", "touched", "selected" };
        static const char *sLabelsPlural[4] = { "records", "deleted", "touched", "selected" };

        std::ostringstream stream;

        bool first = true;

        for (int i=0; i<4; ++i)
        {
            if (mStatusCount[i]>0)
            {
                if (first)
                    first = false;
                else
                    stream << ", ";

                stream
                    << mStatusCount[i] << ' '
                    << (mStatusCount[i]==1 ? sLabels[i] : sLabelsPlural[i]);
            }
        }

        mStatus->setText (QString::fromUtf8 (stream.str().c_str()));
    }
}

CSVWorld::TableBottomBox::TableBottomBox (const CreatorFactoryBase& creatorFactory, QWidget *parent)
: QWidget (parent), mShowStatusBar (false)
{
    for (int i=0; i<4; ++i)
        mStatusCount[i] = 0;

    setVisible (false);

    QStackedLayout *layout = new QStackedLayout;

    mStatus = new QLabel;

    QStatusBar *statusBar = new QStatusBar;

    statusBar->addWidget (mStatus);

    layout->addWidget (statusBar);

    setLayout (layout);

    mCreator = creatorFactory.makeCreator();
}

CSVWorld::TableBottomBox::~TableBottomBox()
{
    delete mCreator;
}

void CSVWorld::TableBottomBox::setStatusBar (bool show)
{
    if (show!=mShowStatusBar)
    {
        setVisible (show);

        mShowStatusBar = show;

        if (show)
            updateStatus();
    }
}

bool CSVWorld::TableBottomBox::canCreateAndDelete() const
{
    return mCreator;
}

void CSVWorld::TableBottomBox::selectionSizeChanged (int size)
{
    if (mStatusCount[3]!=size)
    {
        mStatusCount[3] = size;
        updateStatus();
    }
}

void CSVWorld::TableBottomBox::tableSizeChanged (int size, int deleted, int modified)
{
    bool changed = false;

    if (mStatusCount[0]!=size)
    {
        mStatusCount[0] = size;
        changed = true;
    }

    if (mStatusCount[1]!=deleted)
    {
        mStatusCount[1] = deleted;
        changed = true;
    }

    if (mStatusCount[2]!=modified)
    {
        mStatusCount[2] = modified;
        changed = true;
    }

    if (changed)
        updateStatus();
}