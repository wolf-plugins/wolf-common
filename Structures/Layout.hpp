#pragma once

START_NAMESPACE_DISTRHO

struct Anchors
{
	Anchors();
	Anchors(bool left, bool right, bool top, bool bottom);

	bool left;
	bool right;
	bool top;
	bool bottom;
};

Anchors::Anchors() : left(true),
					 right(false),
					 top(true),
					 bottom(false)
{
}

Anchors::Anchors(bool left, bool right, bool top, bool bottom) : left(left),
																 right(right),
																 top(top),
																 bottom(bottom)
{
}

struct RelativePosition
{
	RelativePosition();
	RelativePosition(int left, int right, int top, int bottom);

	int left;
	int right;
	int top;
	int bottom;
};

RelativePosition::RelativePosition() : left(0),
									   right(0),
									   top(0),
									   bottom(0)
{
}

RelativePosition::RelativePosition(int left, int right, int top, int bottom) : left(left),
																			   right(right),
																			   top(top),
																			   bottom(bottom)
{
}

class Layout;

class LayoutItem
{
  public:
	LayoutItem(Layout *parent, Widget *widget);

	Widget *getWidget();

	LayoutItem &setAnchors(Anchors anchors);
	Anchors getAnchors();

	LayoutItem &setSize(const uint width, const uint height);
	LayoutItem &setPosition(const int x, const int y);

	void setRelativePos(int left, int right, int top, int bottom);
	void setRelativeLeft(int left);
	void setRelativeRight(int right);
	void setRelativeTop(int top);
	void setRelativeBottom(int bottom);

	RelativePosition getRelativePos();

  private:
	Widget *fWidget;
	Layout *fParent;
	Anchors fAnchors;

	// the absolute distance of the item from the sides of the layout
	RelativePosition fRelativePos;
};

LayoutItem::LayoutItem(Layout *parent, Widget *widget) : fWidget(widget),
														 fParent(parent),
														 fAnchors(),
														 fRelativePos()
{
}

Widget *LayoutItem::getWidget()
{
	return fWidget;
}

LayoutItem &LayoutItem::setAnchors(Anchors anchors)
{
	fAnchors = anchors;

	return *this;
}

Anchors LayoutItem::getAnchors()
{
	return fAnchors;
}

void LayoutItem::setRelativePos(int left, int right, int top, int bottom)
{
	fRelativePos.left = left;
	fRelativePos.right = right;
	fRelativePos.top = top;
	fRelativePos.bottom = bottom;
}

void LayoutItem::setRelativeLeft(int left)
{
	fRelativePos.left = left;
}

void LayoutItem::setRelativeRight(int right)
{
	fRelativePos.right = right;
}

void LayoutItem::setRelativeTop(int top)
{
	fRelativePos.top = top;
}

void LayoutItem::setRelativeBottom(int bottom)
{
	fRelativePos.bottom = bottom;
}

RelativePosition LayoutItem::getRelativePos()
{
	return fRelativePos;
}

class Layout : public Widget
{
  public:
	Layout(Widget *parent);
	LayoutItem &addItem(Widget *widget);
	size_t getItemCount();
	LayoutItem *getItem(const int index);

  protected:
	virtual void onItemAdded(const LayoutItem &item);

  private:
	std::vector<LayoutItem> fItems;
};

Layout::Layout(Widget *parent) : Widget(parent)
{
	setSize(parent->getSize());
}

LayoutItem &LayoutItem::setSize(const uint width, const uint height)
{
	fWidget->setSize(width, height);

	fRelativePos.right = fParent->getWidth() - (fRelativePos.left + fWidget->getWidth());
	fRelativePos.bottom = fParent->getHeight() - (fRelativePos.top + fWidget->getHeight());

	return *this;
}

LayoutItem &LayoutItem::setPosition(const int x, const int y)
{
	fRelativePos.left = x;
	fRelativePos.right = fParent->getWidth() - (fRelativePos.left + fWidget->getWidth());
	fRelativePos.top = y;
	fRelativePos.bottom = fParent->getHeight() - (fRelativePos.top + fWidget->getHeight());

	fWidget->setAbsolutePos(fParent->getAbsoluteX() + fRelativePos.left, fParent->getAbsoluteY() + fRelativePos.top);

	return *this;
}

LayoutItem *Layout::getItem(const int index)
{
	return &fItems[index];
}

size_t Layout::getItemCount()
{
	return fItems.size();
}

LayoutItem &Layout::addItem(Widget *widget)
{
	LayoutItem item = LayoutItem(this, widget);

	fItems.push_back(item);

	onItemAdded(item);

	return fItems[fItems.size() - 1];
}

void Layout::onItemAdded(const LayoutItem &item)
{
}

class RelativeLayout : public Layout
{
  public:
	RelativeLayout(Widget *parent);
	void repositionItems(Size<uint> oldSize, Size<uint> newSize);
	void repositionItems();

  protected:
	void onResize(const ResizeEvent &ev) override;
	void onItemAdded(const LayoutItem &item) override;
	void onPositionChanged(const PositionChangedEvent &ev) override;
	void onDisplay() override;

  private:
};

RelativeLayout::RelativeLayout(Widget *parent) : Layout(parent)
{
	hide();
}

void RelativeLayout::onDisplay()
{
}

void RelativeLayout::repositionItems()
{
	repositionItems(getSize(), getSize());
}

void RelativeLayout::repositionItems(Size<uint> oldSize, Size<uint> newSize)
{
	int absX = getAbsoluteX();
	int absY = getAbsoluteY();

	for (int i = 0; i < getItemCount(); ++i)
	{
		LayoutItem *item = getItem(i);
		const Anchors anchors = item->getAnchors();

		int deltaWidth = newSize.getWidth() - oldSize.getWidth();
		int deltaHeight = newSize.getHeight() - oldSize.getHeight();

		if (!anchors.right)
		{
			int right = item->getRelativePos().right;

			item->setRelativeRight(right + deltaWidth);
		}
		else if (!anchors.left)
		{
			int left = item->getRelativePos().left;

			item->setRelativeLeft(left + deltaWidth);
		}

		item->getWidget()->setAbsoluteX(absX + item->getRelativePos().left);
		item->getWidget()->setWidth(getWidth() - item->getRelativePos().left - item->getRelativePos().right);

		if (!anchors.bottom)
		{
			int bottom = item->getRelativePos().bottom;

			item->setRelativeBottom(bottom + deltaHeight);
		}
		else if (!anchors.top)
		{
			int top = item->getRelativePos().top;

			item->setRelativeTop(top + deltaHeight);
		}

		item->getWidget()->setAbsoluteY(absY + item->getRelativePos().top);
		item->getWidget()->setHeight(getHeight() - item->getRelativePos().top - item->getRelativePos().bottom);
	}
}

void RelativeLayout::onResize(const ResizeEvent &ev)
{
	repositionItems(ev.oldSize, ev.size);
}

void RelativeLayout::onPositionChanged(const PositionChangedEvent &ev)
{
	const int absX = getAbsoluteX();
	const int absY = getAbsoluteY();

	for (int i = 0; i < getItemCount(); ++i)
	{
		LayoutItem *item = getItem(i);

		item->getWidget()->setAbsoluteX(absX + item->getRelativePos().left);
		item->getWidget()->setAbsoluteY(absY + item->getRelativePos().top);
	}
}

void RelativeLayout::onItemAdded(const LayoutItem &item)
{
}

END_NAMESPACE_DISTRHO
