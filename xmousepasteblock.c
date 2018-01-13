/*
 * Copyright (c) 2018 Micha LaQua <micha.laqua@gmail.com>
 *
 * Special thanks to Ingo Buerk (Airblader) for his work on the
 * awesome unclutter-xfixes project, upon which the XInput eventcode
 * is based on.
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ev.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/XInput2.h>

static Display *display;
static struct ev_io *x_watcher;
static struct ev_check *x_check;

int xi_opcode = -1;

void errormsg(char *msg) {
    printf("ERROR: %s\n", msg);
    exit(1);
}

void init_xinput(void) {
    int event, error;
    if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &event, &error)) {
        errormsg("XInput extension not available");
    }

    int major_op = 2, minor_op = 2;
    int result = XIQueryVersion(display, &major_op, &minor_op);
    if (result == BadRequest) {
        errormsg("XI2 is not supported in a sufficient version (>=2.2 required).");
    } else if (result != Success) {
        errormsg("Failed to query XI2");
    }
}

static void init_eventmask(void) {
    XIEventMask masks[1];
    unsigned char mask[(XI_LASTEVENT + 7)/8];

    memset(mask, 0, sizeof(mask));
    masks[0].deviceid = XIAllMasterDevices;
    masks[0].mask_len = sizeof(mask);
    masks[0].mask = mask;

    XISetMask(mask, XI_RawButtonPress);

    XISelectEvents(display, DefaultRootWindow(display), masks, 1);
    XFlush(display);
}

int clear_primary(void) {
    XSetSelectionOwner(display, XA_PRIMARY, None, CurrentTime);
    XSync(display, False);
#ifdef DEBUG
    printf("primary selection cleared\n");
#endif
}

static void stub_cb(EV_P_ ev_io *w, int revents) {
    /* STUB */
}

static void check_cb(EV_P_ ev_check *w, int revents) {
    XEvent ev;
    while (XPending(display) > 0) {
        XNextEvent(display, &ev);
        XGenericEventCookie *cookie = &ev.xcookie;
        if (cookie->type != GenericEvent || cookie->extension !=  xi_opcode || !XGetEventData(display, cookie)) {
            continue;
        }

        const XIRawEvent *data = (const XIRawEvent *) cookie->data;
#ifdef DEBUG
        printf("button %i pressed\n", data->detail);
#endif
        if (data->detail == 2) {
            clear_primary();
        }

        XFreeEventData(display, cookie);
    }
}

int main(int argc, char** argv) {
    struct ev_loop *evloop;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        errormsg("Failed to connect to the X server");
        return 1;
    }

    init_xinput();
    init_eventmask();

    evloop = EV_DEFAULT;
    
    x_watcher = calloc(sizeof(struct ev_io), 1);
    ev_io_init(x_watcher, stub_cb, XConnectionNumber(display), EV_READ);
    ev_io_start(evloop, x_watcher);
    
    x_check = calloc(sizeof(struct ev_check), 1);
    ev_check_init(x_check, check_cb);
    ev_check_start(evloop, x_check);

    ev_run(evloop, 0);

    return 0;
}
