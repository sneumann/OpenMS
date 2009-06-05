// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework 
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2009 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Johannes Junker $
// $Authors: Johannes Junker $
// --------------------------------------------------------------------------

#ifndef OPENMS_VISUAL_TOPPASEDGE_H
#define OPENMS_VISUAL_TOPPASEDGE_H

#include <OpenMS/config.h>

#include <QtGui/QGraphicsItem>

namespace OpenMS
{
	class TOPPASVertex;
	
	class OPENMS_DLLAPI TOPPASEdge
		: public QObject,
			public QGraphicsItem
	{
		Q_OBJECT
		
		public:
			
			/// The type of this edge
			enum EdgeType
			{
				ET_FILE_TO_TOOL,
				ET_LIST_TO_TOOL,
				ET_TOOL_TO_FILE,
				ET_TOOL_TO_LIST,
				ET_TOOL_TO_TOOL,
				ET_INVALID
			};
			
			/// Is a hypothetical edge valid or not? Does something have to be configured?
			enum EdgeValidity
			{
				EV_RED,			// not allowed
				EV_YELLOW,	// allowed, but must be configured first
				EV_GREEN		// allowed and unambiguous
			};
			
			/// Standard constructor
			TOPPASEdge();
			/// Constructor
			TOPPASEdge(TOPPASVertex* from, const QPointF& hover_pos);
			/// Copy constructor
			TOPPASEdge(const TOPPASEdge& rhs);
			/// Destructor
			virtual ~TOPPASEdge();
			/// Assignment operator
			TOPPASEdge& operator= (const TOPPASEdge& rhs);
			
			/// Returns the bounding rectangle of this item
			QRectF boundingRect() const;
			/// Returns a more precise shape
			QPainterPath shape () const;
			/// Paints the item
			void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
			/// Returns the start position of this edge
			QPointF startPos() const;
			/// Returns the end position of this edge
			QPointF endPos() const;
			/// Sets the position of the hovering end while edge is being created
			void setHoverPos(const QPointF& pos);
			/// Sets the source vertex of this edge
			void setSourceVertex(TOPPASVertex* tv);
			/// Sets the target vertex of this edge
			void setTargetVertex(TOPPASVertex* tv);
			/// Returns the source vertex
			TOPPASVertex* getSourceVertex();
			/// Returns the target vertex
			TOPPASVertex* getTargetVertex();
			/// Call this before changing the item geometry
			void prepareResize();
			/// Sets the color
			void setColor(const QColor& color);
			/// Determines the type of this edge
			void determineEdgeType();
			/// Returns the type of this edge
			EdgeType getEdgeType();
			
		protected:
			
			///@name reimplemented Qt events
      //@{
      void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);
			//@}
			
			/// Returns the point in the @p list that is nearest to @p origin
			QPointF nearestPoint_(const QPointF& origin, const QList<QPointF>& list) const;
			/// Pointer to the source of this edge
			TOPPASVertex* from_;
			/// Pointer to the target of this edge
			TOPPASVertex* to_;
			/// Position of hovering end while edge is being created
			QPointF hover_pos_;
			/// The color
			QColor color_;
			/// The type of this edge
			EdgeType edge_type_;
	};
}

#endif
