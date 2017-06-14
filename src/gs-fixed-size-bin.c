/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2016 Rafał Lużyński <digitalfreak@lingonborough.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"

#include "gs-fixed-size-bin.h"

struct _GsFixedSizeBin {
	GtkBin parent;
	gint preferred_width;
	gint preferred_height;
	gint min_width;
	gint min_height;
};

G_DEFINE_TYPE (GsFixedSizeBin, gs_fixed_size_bin, GTK_TYPE_BIN)

enum {
	PROP_0,
	PROP_PREFERRED_WIDTH,
	PROP_PREFERRED_HEIGHT
};

static void
gs_fixed_size_bin_size_allocate (GtkWidget           *widget,
				 const GtkAllocation *allocation,
				 int                  baseline,
				 GtkAllocation       *out_clip)
{
	GsFixedSizeBin *bin = GS_FIXED_SIZE_BIN (widget);

	/*
	 * TODO: I'm not exactly sure what this widget does.
	 */



	if (bin->preferred_width >= 0 &&
			bin->preferred_width >= bin->min_width &&
			allocation->width > bin->preferred_width) {
		/* Center the contents */
		/*allocation->x += (allocation->width - bin->preferred_width) / 2;*/
		/*allocation->width = bin->preferred_width;*/
	}
	if (bin->preferred_height >= 0 &&
			bin->preferred_height >= bin->min_height &&
			allocation->height > bin->preferred_height) {
		/* Align to the top */
		/*allocation->height = bin->preferred_height;*/
	}

	GTK_WIDGET_CLASS (gs_fixed_size_bin_parent_class)->size_allocate (widget, allocation, baseline, out_clip);
}

static void
gs_fixed_size_bin_measure (GtkWidget      *widget,
                           GtkOrientation  orientation,
                           int             for_size,
                           int            *minimum,
                           int            *natural,
                           int            *minimum_baseline,
                           int            *natural_baseline)
{
	GsFixedSizeBin *bin = GS_FIXED_SIZE_BIN (widget);
	int m, n;

	if (orientation == GTK_ORIENTATION_HORIZONTAL &&
	    bin->preferred_height >= 0 &&
	    for_size > bin->preferred_height) {
		for_size = MAX (bin->min_height, bin->preferred_height);
	} else if (orientation == GTK_ORIENTATION_VERTICAL &&
	           bin->preferred_width >= 0 &&
	           for_size > bin->preferred_width) {
		for_size = MAX (bin->min_width, bin->preferred_width);
	}

	GTK_WIDGET_CLASS (gs_fixed_size_bin_parent_class)->measure (widget,
	                                                            orientation,
	                                                            for_size,
	                                                            &m, &n,
	                                                            NULL, NULL);

	if (orientation == GTK_ORIENTATION_HORIZONTAL) {
		bin->min_width = m;
		if (bin->preferred_width >= 0 && n > bin->preferred_width)
			n = MAX (m, bin->preferred_width);
	} else {
		bin->min_height = m;
		if (bin->preferred_height >= 0 && n > bin->preferred_height)
			n = MAX (m, bin->preferred_height);
	}

	*minimum = m;
	*natural = n;
}

static void
gs_fixed_size_bin_get_property (GObject *object,
				guint prop_id,
				GValue *value,
				GParamSpec *pspec)
{
	GsFixedSizeBin *bin = GS_FIXED_SIZE_BIN (object);

	switch (prop_id) {
	case PROP_PREFERRED_WIDTH:
		g_value_set_int (value, bin->preferred_width);
		break;
	case PROP_PREFERRED_HEIGHT:
		g_value_set_int (value, bin->preferred_height);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gs_fixed_size_bin_set_property (GObject *object,
				guint prop_id,
				const GValue *value,
				GParamSpec *pspec)
{
	GsFixedSizeBin *bin = GS_FIXED_SIZE_BIN (object);

	switch (prop_id) {
	case PROP_PREFERRED_WIDTH:
		bin->preferred_width = g_value_get_int (value);
		gtk_widget_queue_resize (GTK_WIDGET (bin));
		break;
	case PROP_PREFERRED_HEIGHT:
		bin->preferred_height = g_value_get_int (value);
		gtk_widget_queue_resize (GTK_WIDGET (bin));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gs_fixed_size_bin_init (GsFixedSizeBin *bin)
{
	gtk_widget_set_has_window (GTK_WIDGET (bin), FALSE);
	bin->preferred_width = -1;
	bin->preferred_height = -1;
}

static void
gs_fixed_size_bin_class_init (GsFixedSizeBinClass *class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

	object_class->get_property = gs_fixed_size_bin_get_property;
	object_class->set_property = gs_fixed_size_bin_set_property;

	widget_class->size_allocate = gs_fixed_size_bin_size_allocate;
	widget_class->measure = gs_fixed_size_bin_measure;

	g_object_class_install_property (object_class, PROP_PREFERRED_WIDTH,
		g_param_spec_int ("preferred-width",
				  "Preferred width",
				  "The width of this widget unless its parent is smaller or its child requires more",
				  -1, G_MAXINT, -1,
				  G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_PREFERRED_HEIGHT,
		g_param_spec_int ("preferred-height",
				  "Preferred height",
				  "The height of this widget unless its parent is smaller or its child requires more",
				  -1, G_MAXINT, -1,
				  G_PARAM_READWRITE));
}

GtkWidget *
gs_fixed_size_bin_new (void)
{
	return g_object_new (GS_TYPE_FIXED_SIZE_BIN, NULL);
}

/* vim: set noexpandtab: */
