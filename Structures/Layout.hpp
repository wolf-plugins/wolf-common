struct Anchors 
{
	Anchors();
	Anchors(bool left, bool right, bool top, bool bottom);

	bool left;
	bool right;
	bool top;
	bool bottom;
}

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

class LayoutItem : public Widget
{
	public:
		LayoutItem();
		Anchors getAnchors();
		void setRelativePos(int left, int right, int top, int bottom);
		void setRelativeLeft(int left);
		void setRelativeRight(int right);
		void getRelativePos();

	private:
		Anchors fAnchors;

		// the absolute distance of the item from the sides of the layout
		RelativePosition fRelativePos;
}

LayoutItem::LayoutItem() : Widget(),
			   fAnchors(),
			   fRelativePos()
{

}

LayoutItem::setRelativePos(int left, int right, int top, int bottom)
{
	fRelativePos.left = left;
	fRelativePos.right = right;
	fRelativePos.top = top;
	fRelativePos.bottom = bottom;
}

LayoutItem::setRelativeLeft(int left)
{
	fRelativePos.left = left;
}

LayoutItem::setRelativeRight(int right)
{
	fRelativePos.right = right;
}

RelativePosition LayoutItem::getRelativePos()
{
	return fRelativePos;
}

class Layout : public Widget
{
	public:
		Layout();
		void addItem(LayoutItem *widget);
		int getItemCount();

	protected:
		std::vector<LayoutItem*> getItems();
		virtual void onItemAdded(LayoutItem *item);

	private:
		std::vector<LayoutItem*> fItems;
};

Layout::Layout() : Widget()
{

}

LayoutItem* Layout::getItem(const int index)
{
	return fItems[index];
}

LayoutItem* Layout::getItemCount()
{
	return fItems.count();
}

void Layout::addItem(LayoutItem *item)
{
	fItems.push(item);

	onItemAdded(item);
}

void Layout::onItemAdded()
{

}

class AnchorsLayout : public Layout
{
	public:
		AnchorsLayout();

	protected:
		void onResize(const ResizeEvent &ev) override;
		void onItemAdded(LayoutItem *item) override;
		void onPositionChanged(const PosChangedEvent &ev) override;

	private:
		// Change the widgets' position after a resize
		void repositionItems(Size<uint> oldSize, Size<uint> newSize);
};

AnchorsLayout::AnchorsLayout() : Layout()
{

}

void AnchorsLayout::repositionItems(Size<uint> oldSize, Size<uint> newSize)
{
	int absX = getAbsoluteX();
	int absY = getAbsoluteY();

	for(int i = 0; i < getItemCount(); ++i)
	{
		LayoutItem* item = getItem(i);
		const Anchors anchors = item->getAnchors();

		int deltaWidth = newSize.getWidth() -  oldSize.getWidth();
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

		item->setAbsoluteX(absX + item->getRelativePos().left);
		item->setWidth(getWidth() - item->getRelativePos().left - item->getRelativePos().right);

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

		item->setAbsoluteY(absY + item->getRelativePos().top);
		item->setHeight(getHeight() - item->getRelativePos().top - item->getRelativePos().bottom);
	}
}

void AnchorsLayout::onResize(const ResizeEvent &ev)
{
	repositionItems();
}

void AnchorsLayout::onPositionChanged(const PosChangedEvent &ev)
{
	int absX = getAbsoluteX();
	int absY = getAbsoluteY();

	for(int i = 0; i < getItemCount(); ++i)
	{
		LayoutItem* item = getItem(i);

		item->setAbsoluteX(absX + item->getRelativePos().left);
		item->setAbsoluteY(absY + item->getRelativePos().top);
	}
}

void onItemAdded(LayoutItem *item)
{
	repositionItems();
}
