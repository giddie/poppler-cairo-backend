//========================================================================
//
// annot_stamp_approved.h
//
// Copyright (C) 2021 Mahmoud Ahmed Khalil <mahmoudkhalil11@gmail.com>
// Copyright (C) 2021 Albert Astals Cid <aacid@kde.org>
//
// Mechanically extracted from an SVG created for Okular by Eugene Trounev eugene.trounev@gmail.com
//
// Licensed under GPLv2 or later
//
//========================================================================

#ifndef ANNOT_STAMP_APPROVED_H
#define ANNOT_STAMP_APPROVED_H

#include "Dict.h"
#include "Object.h"
#include "PDFDoc.h"

static const double ANNOT_STAMP_APPROVED_WIDTH = 127.008179;
static const double ANNOT_STAMP_APPROVED_HEIGHT = 26.484743;

static const char *ANNOT_STAMP_APPROVED = "1 0 0 -1 0 26.484744 cm\n"
                                          "q\n"
                                          "1 1 1 rg /a0 gs\n"
                                          "3.406 2.129 m 123.617 2.129 l 124.316 2.129 124.887 2.828 124.887 3.398\n"
                                          " c 124.887 23.09 l 124.887 23.789 124.32 24.359 123.617 24.359 c 3.406 24.359\n"
                                          " l 2.707 24.359 2.137 23.66 2.137 23.09 c 2.137 3.398 l 2.137 2.699 2.703\n"
                                          " 2.129 3.406 2.129 c h\n"
                                          "3.406 2.129 m f\n"
                                          "0 0.298039 0.431373 rg /a1 gs\n"
                                          "23.453 20.828 m 22.086 16.895 l 16.219 16.895 l 14.852 20.828 l 11.629 \n"
                                          "20.828 l 17.246 5.434 l 21.047 5.434 l 26.641 20.828 l 23.449 20.828 l 19.844\n"
                                          " 10.043 m 19.758 9.789 19.668 9.531 19.582 9.277 c 19.5 9.016 19.43 8.777\n"
                                          " 19.363 8.566 c 19.305 8.348 19.254 8.168 19.211 8.031 c 19.176 7.895 19.152\n"
                                          " 7.816 19.145 7.801 c 19.137 7.824 19.117 7.902 19.078 8.043 c 19.043 8.18\n"
                                          " 18.992 8.355 18.926 8.566 c 18.867 8.777 18.793 9.016 18.707 9.277 c 18.625\n"
                                          " 9.531 18.543 9.789 18.457 10.043 c 16.938 14.469 l 21.363 14.469 l 19.844\n"
                                          " 10.043 l 39.992 14.863 m 39.992 15.758 39.902 16.586 39.719 17.344 c 39.543\n"
                                          " 18.102 39.27 18.754 38.898 19.301 c 38.527 19.848 38.055 20.277 37.477 \n"
                                          "20.59 c 36.91 20.895 36.234 21.047 35.457 21.047 c 35.105 21.047 34.758 \n"
                                          "21.012 34.406 20.938 c 34.062 20.863 33.734 20.75 33.422 20.586 c 33.109\n"
                                          " 20.418 32.816 20.203 32.547 19.941 c 32.285 19.672 32.059 19.34 31.871 \n"
                                          "18.945 c 31.805 18.945 l 31.812 18.98 31.82 19.074 31.828 19.219 c 31.836\n"
                                          " 19.363 31.844 19.535 31.852 19.73 c 31.859 19.918 31.863 20.125 31.863 \n"
                                          "20.344 c 31.871 20.555 31.875 20.75 31.875 20.934 c 31.875 25.469 l 28.805\n"
                                          " 25.469 l 28.805 11.723 l 28.805 11.117 28.793 10.578 28.773 10.105 c 28.758\n"
                                          " 9.633 28.742 9.266 28.719 9 c 31.703 9 l 31.719 9.051 31.73 9.148 31.746\n"
                                          " 9.297 c 31.77 9.441 31.781 9.609 31.789 9.801 c 31.805 9.988 31.816 10.188\n"
                                          " 31.82 10.391 c 31.828 10.594 31.832 10.773 31.832 10.926 c 31.875 10.926\n"
                                          " l 32.246 10.145 32.758 9.59 33.406 9.254 c 34.055 8.918 34.805 8.75 35.656\n"
                                          " 8.75 c 36.406 8.75 37.055 8.902 37.602 9.207 c 38.148 9.512 38.598 9.934\n"
                                          " 38.945 10.473 c 39.301 11.012 39.566 11.656 39.73 12.406 c 39.906 13.148\n"
                                          " 39.992 13.965 39.992 14.855 c 36.789 14.855 m 36.789 13.508 36.586 12.512\n"
                                          " 36.176 11.863 c 35.77 11.207 35.16 10.879 34.352 10.879 c 34.047 10.879\n"
                                          " 33.738 10.945 33.434 11.074 c 33.137 11.199 32.867 11.418 32.625 11.73 \n"
                                          "c 32.391 12.035 32.199 12.453 32.047 12.977 c 31.902 13.492 31.828 14.148\n"
                                          " 31.828 14.945 c 31.828 15.719 31.902 16.363 32.047 16.879 c 32.191 17.391\n"
                                          " 32.383 17.797 32.613 18.102 c 32.855 18.406 33.125 18.625 33.422 18.758\n"
                                          " c 33.719 18.883 34.023 18.945 34.328 18.945 c 34.723 18.945 35.07 18.867\n"
                                          " 35.379 18.715 c 35.684 18.555 35.941 18.312 36.145 17.984 c 36.355 17.648\n"
                                          " 36.516 17.223 36.625 16.707 c 36.734 16.191 36.789 15.574 36.789 14.859\n"
                                          " c 53.672 14.859 m 53.672 15.754 53.582 16.582 53.398 17.34 c 53.223 18.098\n"
                                          " 52.949 18.75 52.578 19.297 c 52.207 19.844 51.734 20.273 51.156 20.586 \n"
                                          "c 50.59 20.891 49.914 21.043 49.137 21.043 c 48.785 21.043 48.438 21.008\n"
                                          " 48.086 20.934 c 47.742 20.859 47.414 20.746 47.102 20.582 c 46.789 20.414\n"
                                          " 46.496 20.199 46.227 19.938 c 45.965 19.668 45.738 19.336 45.551 18.941\n"
                                          " c 45.484 18.941 l 45.492 18.977 45.5 19.07 45.508 19.215 c 45.516 19.359\n"
                                          " 45.523 19.531 45.531 19.727 c 45.539 19.914 45.543 20.121 45.543 20.34 \n"
                                          "c 45.551 20.551 45.555 20.746 45.555 20.93 c 45.555 25.465 l 42.484 25.465\n"
                                          " l 42.484 11.719 l 42.484 11.113 42.473 10.574 42.453 10.102 c 42.438 9.629\n"
                                          " 42.422 9.262 42.398 8.996 c 45.383 8.996 l 45.398 9.047 45.41 9.145 45.426\n"
                                          " 9.293 c 45.449 9.438 45.461 9.605 45.469 9.797 c 45.484 9.984 45.496 10.184\n"
                                          " 45.5 10.387 c 45.508 10.59 45.512 10.77 45.512 10.922 c 45.555 10.922 l\n"
                                          " 45.926 10.141 46.438 9.586 47.086 9.25 c 47.734 8.914 48.484 8.746 49.336\n"
                                          " 8.746 c 50.086 8.746 50.734 8.898 51.281 9.203 c 51.828 9.508 52.277 9.93\n"
                                          " 52.625 10.469 c 52.98 11.008 53.246 11.652 53.41 12.402 c 53.586 13.145\n"
                                          " 53.672 13.961 53.672 14.852 c 50.469 14.852 m 50.469 13.504 50.266 12.508\n"
                                          " 49.855 11.859 c 49.449 11.203 48.84 10.875 48.031 10.875 c 47.727 10.875\n"
                                          " 47.418 10.941 47.113 11.07 c 46.816 11.195 46.547 11.414 46.305 11.727 \n"
                                          "c 46.07 12.031 45.879 12.449 45.727 12.973 c 45.582 13.488 45.508 14.145\n"
                                          " 45.508 14.941 c 45.508 15.715 45.582 16.359 45.727 16.875 c 45.871 17.387\n"
                                          " 46.062 17.793 46.293 18.098 c 46.535 18.402 46.805 18.621 47.102 18.754\n"
                                          " c 47.398 18.879 47.703 18.941 48.008 18.941 c 48.402 18.941 48.75 18.863\n"
                                          " 49.059 18.711 c 49.363 18.551 49.621 18.309 49.824 17.98 c 50.035 17.645\n"
                                          " 50.195 17.219 50.305 16.703 c 50.414 16.188 50.469 15.57 50.469 14.855 \n"
                                          "c 56.16 20.82 m 56.16 11.773 l 56.16 11.52 56.156 11.25 56.148 10.965 c \n"
                                          "56.148 10.68 56.141 10.41 56.125 10.156 c 56.117 9.895 56.109 9.66 56.102\n"
                                          " 9.457 c 56.094 9.246 56.082 9.094 56.07 9 c 59 9 l 59.016 9.086 59.027 \n"
                                          "9.242 59.043 9.457 c 59.059 9.668 59.07 9.902 59.086 10.156 c 59.102 10.41\n"
                                          " 59.113 10.668 59.117 10.922 c 59.133 11.168 59.141 11.371 59.141 11.523\n"
                                          " c 59.184 11.523 l 59.336 11.094 59.488 10.711 59.641 10.375 c 59.793 10.031\n"
                                          " 59.973 9.746 60.176 9.512 c 60.387 9.27 60.637 9.09 60.93 8.965 c 61.223\n"
                                          " 8.832 61.582 8.77 62.012 8.77 c 62.195 8.77 62.371 8.789 62.547 8.824 c\n"
                                          " 62.73 8.852 62.867 8.891 62.961 8.934 c 62.961 11.5 l 62.766 11.457 62.562\n"
                                          " 11.418 62.359 11.391 c 62.164 11.355 61.926 11.336 61.648 11.336 c 60.883\n"
                                          " 11.336 60.285 11.645 59.855 12.266 c 59.434 12.887 59.223 13.801 59.223\n"
                                          " 15.02 c 59.223 20.82 l 56.152 20.82 l 76.082 14.898 m 76.082 15.801 75.953\n"
                                          " 16.629 75.699 17.379 c 75.453 18.129 75.078 18.777 74.574 19.324 c 74.07\n"
                                          " 19.863 73.445 20.285 72.695 20.59 c 71.945 20.887 71.07 21.039 70.074 21.039\n"
                                          " c 69.113 21.039 68.266 20.891 67.527 20.59 c 66.793 20.293 66.172 19.871\n"
                                          " 65.668 19.332 c 65.172 18.793 64.797 18.148 64.543 17.398 c 64.289 16.641\n"
                                          " 64.16 15.809 64.16 14.895 c 64.16 14.012 64.281 13.199 64.52 12.457 c 64.766\n"
                                          " 11.707 65.141 11.059 65.633 10.512 c 66.129 9.965 66.75 9.539 67.5 9.234\n"
                                          " c 68.25 8.93 69.129 8.777 70.133 8.777 c 71.195 8.777 72.102 8.93 72.855\n"
                                          " 9.234 c 73.605 9.539 74.219 9.965 74.691 10.512 c 75.172 11.051 75.523 \n"
                                          "11.695 75.742 12.445 c 75.969 13.188 76.082 14.004 76.082 14.895 c 72.871\n"
                                          " 14.895 m 72.871 13.488 72.641 12.469 72.184 11.836 c 71.727 11.203 71.059\n"
                                          " 10.887 70.184 10.887 c 69.281 10.887 68.59 11.207 68.109 11.848 c 67.629\n"
                                          " 12.488 67.387 13.504 67.387 14.895 c 67.387 15.602 67.449 16.211 67.574\n"
                                          " 16.719 c 67.707 17.23 67.887 17.648 68.121 17.977 c 68.355 18.305 68.633\n"
                                          " 18.547 68.961 18.707 c 69.289 18.859 69.648 18.938 70.043 18.938 c 70.496\n"
                                          " 18.938 70.895 18.859 71.246 18.707 c 71.602 18.547 71.902 18.305 72.141\n"
                                          " 17.977 c 72.383 17.648 72.562 17.23 72.688 16.719 c 72.812 16.207 72.875\n"
                                          " 15.602 72.875 14.895 c 84.961 20.816 m 81.289 20.816 l 77.059 8.992 l 80.305\n"
                                          " 8.992 l 82.371 15.602 l 82.438 15.82 82.504 16.055 82.578 16.301 c 82.652\n"
                                          " 16.543 82.719 16.781 82.785 17.023 c 82.852 17.266 82.914 17.496 82.973\n"
                                          " 17.723 c 83.039 17.949 83.098 18.152 83.148 18.336 c 83.191 18.16 83.246\n"
                                          " 17.965 83.312 17.746 c 83.379 17.52 83.445 17.289 83.508 17.047 c 83.582\n"
                                          " 16.805 83.652 16.566 83.727 16.324 c 83.809 16.082 83.883 15.855 83.957\n"
                                          " 15.637 c 86.109 8.992 l 89.32 8.992 l 84.961 20.816 l 95.832 21.035 m 94.98\n"
                                          " 21.035 94.211 20.91 93.527 20.664 c 92.852 20.41 92.27 20.027 91.789 19.516\n"
                                          " c 91.309 19 90.941 18.355 90.684 17.582 c 90.43 16.801 90.301 15.891 90.301\n"
                                          " 14.852 c 90.301 13.723 90.449 12.773 90.75 12 c 91.055 11.227 91.465 10.605\n"
                                          " 91.973 10.133 c 92.488 9.652 93.082 9.305 93.754 9.094 c 94.426 8.883 95.129\n"
                                          " 8.777 95.875 8.777 c 96.809 8.777 97.602 8.941 98.258 9.27 c 98.922 9.59\n"
                                          " 99.465 10.043 99.887 10.625 c 100.309 11.207 100.617 11.906 100.816 12.723\n"
                                          " c 101.012 13.531 101.113 14.426 101.113 15.41 c 101.113 15.496 l 93.539\n"
                                          " 15.508 l 93.539 16.004 93.582 16.465 93.672 16.895 c 93.758 17.316 93.902\n"
                                          " 17.684 94.098 18 c 94.293 18.305 94.551 18.551 94.863 18.73 c 95.176 18.906\n"
                                          " 95.555 18.992 96 18.992 c 96.539 18.992 96.98 18.879 97.32 18.652 c 97.664\n"
                                          " 18.418 97.906 18.062 98.051 17.582 c 100.945 17.832 l 100.812 18.168 100.629\n"
                                          " 18.523 100.387 18.902 c 100.152 19.281 99.84 19.629 99.445 19.953 c 99.051\n"
                                          " 20.266 98.559 20.527 97.969 20.738 c 97.387 20.941 96.676 21.043 95.84 \n"
                                          "21.043 c 95.84 10.719 m 95.527 10.719 95.23 10.773 94.953 10.883 c 94.684\n"
                                          " 10.984 94.445 11.152 94.242 11.387 c 94.047 11.613 93.887 11.906 93.762\n"
                                          " 12.273 c 93.637 12.637 93.57 13.074 93.555 13.586 c 98.145 13.586 l 98.086\n"
                                          " 12.633 97.855 11.918 97.457 11.445 c 97.055 10.965 96.516 10.723 95.84 \n"
                                          "10.723 c 111.117 20.828 m 111.102 20.77 111.086 20.668 111.062 20.523 c \n"
                                          "111.047 20.371 111.031 20.199 111.008 20.012 c 110.992 19.824 110.98 19.629\n"
                                          " 110.965 19.434 c 110.957 19.238 110.953 19.062 110.953 18.91 c 110.91 18.91\n"
                                          " l 110.555 19.676 110.055 20.227 109.414 20.559 c 108.781 20.887 108.02 \n"
                                          "21.051 107.129 21.051 c 106.387 21.051 105.742 20.898 105.195 20.594 c 104.656\n"
                                          " 20.289 104.207 19.863 103.852 19.316 c 103.5 18.77 103.238 18.125 103.066\n"
                                          " 17.383 c 102.898 16.633 102.816 15.816 102.816 14.934 c 102.816 14.039 \n"
                                          "102.902 13.215 103.078 12.465 c 103.262 11.715 103.535 11.07 103.91 10.531\n"
                                          " c 104.281 9.984 104.75 9.559 105.32 9.254 c 105.895 8.949 106.578 8.797\n"
                                          " 107.363 8.797 c 107.75 8.797 108.121 8.836 108.477 8.918 c 108.832 9 109.164\n"
                                          " 9.121 109.473 9.289 c 109.777 9.457 110.055 9.672 110.305 9.934 c 110.551\n"
                                          " 10.195 110.762 10.512 110.938 10.883 c 110.961 10.883 l 110.961 10.809 \n"
                                          "110.957 10.703 110.949 10.566 c 110.949 10.422 110.949 10.258 110.949 10.074\n"
                                          " c 110.949 9.891 110.945 9.703 110.938 9.508 c 110.938 9.312 110.938 9.121\n"
                                          " 110.938 8.941 c 110.938 4.625 l 114.008 4.625 l 114.008 18.262 l 114.008\n"
                                          " 18.836 114.02 19.352 114.039 19.801 c 114.062 20.246 114.078 20.59 114.094\n"
                                          " 20.84 c 111.121 20.84 l 110.98 14.863 m 110.98 14.082 110.906 13.438 110.762\n"
                                          " 12.93 c 110.617 12.414 110.422 12.004 110.184 11.707 c 109.949 11.402 109.68\n"
                                          " 11.191 109.375 11.074 c 109.078 10.949 108.77 10.887 108.457 10.887 c 108.062\n"
                                          " 10.887 107.715 10.965 107.406 11.117 c 107.109 11.27 106.852 11.512 106.641\n"
                                          " 11.84 c 106.438 12.168 106.281 12.586 106.172 13.098 c 106.07 13.609 106.02\n"
                                          " 14.223 106.02 14.945 c 106.02 17.625 106.824 18.965 108.434 18.965 c 108.738\n"
                                          " 18.965 109.047 18.898 109.352 18.77 c 109.656 18.637 109.93 18.418 110.172\n"
                                          " 18.102 c 110.414 17.789 110.605 17.371 110.75 16.844 c 110.902 16.312 110.98\n"
                                          " 15.652 110.98 14.867 c f\n"
                                          "1 1 1 RG 0.531496 w\n"
                                          "0 J\n"
                                          "0 j\n"
                                          "[] 0.0 d\n"
                                          "4 M q 1 0 0 1 0 0 cm\n"
                                          "23.453 20.828 m 22.086 16.895 l 16.219 16.895 l 14.852 20.828 l 11.629 \n"
                                          "20.828 l 17.246 5.434 l 21.047 5.434 l 26.641 20.828 l 23.449 20.828 l 19.844\n"
                                          " 10.043 m 19.758 9.789 19.668 9.531 19.582 9.277 c 19.5 9.016 19.43 8.777\n"
                                          " 19.363 8.566 c 19.305 8.348 19.254 8.168 19.211 8.031 c 19.176 7.895 19.152\n"
                                          " 7.816 19.145 7.801 c 19.137 7.824 19.117 7.902 19.078 8.043 c 19.043 8.18\n"
                                          " 18.992 8.355 18.926 8.566 c 18.867 8.777 18.793 9.016 18.707 9.277 c 18.625\n"
                                          " 9.531 18.543 9.789 18.457 10.043 c 16.938 14.469 l 21.363 14.469 l 19.844\n"
                                          " 10.043 l 39.992 14.863 m 39.992 15.758 39.902 16.586 39.719 17.344 c 39.543\n"
                                          " 18.102 39.27 18.754 38.898 19.301 c 38.527 19.848 38.055 20.277 37.477 \n"
                                          "20.59 c 36.91 20.895 36.234 21.047 35.457 21.047 c 35.105 21.047 34.758 \n"
                                          "21.012 34.406 20.938 c 34.062 20.863 33.734 20.75 33.422 20.586 c 33.109\n"
                                          " 20.418 32.816 20.203 32.547 19.941 c 32.285 19.672 32.059 19.34 31.871 \n"
                                          "18.945 c 31.805 18.945 l 31.812 18.98 31.82 19.074 31.828 19.219 c 31.836\n"
                                          " 19.363 31.844 19.535 31.852 19.73 c 31.859 19.918 31.863 20.125 31.863 \n"
                                          "20.344 c 31.871 20.555 31.875 20.75 31.875 20.934 c 31.875 25.469 l 28.805\n"
                                          " 25.469 l 28.805 11.723 l 28.805 11.117 28.793 10.578 28.773 10.105 c 28.758\n"
                                          " 9.633 28.742 9.266 28.719 9 c 31.703 9 l 31.719 9.051 31.73 9.148 31.746\n"
                                          " 9.297 c 31.77 9.441 31.781 9.609 31.789 9.801 c 31.805 9.988 31.816 10.188\n"
                                          " 31.82 10.391 c 31.828 10.594 31.832 10.773 31.832 10.926 c 31.875 10.926\n"
                                          " l 32.246 10.145 32.758 9.59 33.406 9.254 c 34.055 8.918 34.805 8.75 35.656\n"
                                          " 8.75 c 36.406 8.75 37.055 8.902 37.602 9.207 c 38.148 9.512 38.598 9.934\n"
                                          " 38.945 10.473 c 39.301 11.012 39.566 11.656 39.73 12.406 c 39.906 13.148\n"
                                          " 39.992 13.965 39.992 14.855 c 36.789 14.855 m 36.789 13.508 36.586 12.512\n"
                                          " 36.176 11.863 c 35.77 11.207 35.16 10.879 34.352 10.879 c 34.047 10.879\n"
                                          " 33.738 10.945 33.434 11.074 c 33.137 11.199 32.867 11.418 32.625 11.73 \n"
                                          "c 32.391 12.035 32.199 12.453 32.047 12.977 c 31.902 13.492 31.828 14.148\n"
                                          " 31.828 14.945 c 31.828 15.719 31.902 16.363 32.047 16.879 c 32.191 17.391\n"
                                          " 32.383 17.797 32.613 18.102 c 32.855 18.406 33.125 18.625 33.422 18.758\n"
                                          " c 33.719 18.883 34.023 18.945 34.328 18.945 c 34.723 18.945 35.07 18.867\n"
                                          " 35.379 18.715 c 35.684 18.555 35.941 18.312 36.145 17.984 c 36.355 17.648\n"
                                          " 36.516 17.223 36.625 16.707 c 36.734 16.191 36.789 15.574 36.789 14.859\n"
                                          " c 53.672 14.859 m 53.672 15.754 53.582 16.582 53.398 17.34 c 53.223 18.098\n"
                                          " 52.949 18.75 52.578 19.297 c 52.207 19.844 51.734 20.273 51.156 20.586 \n"
                                          "c 50.59 20.891 49.914 21.043 49.137 21.043 c 48.785 21.043 48.438 21.008\n"
                                          " 48.086 20.934 c 47.742 20.859 47.414 20.746 47.102 20.582 c 46.789 20.414\n"
                                          " 46.496 20.199 46.227 19.938 c 45.965 19.668 45.738 19.336 45.551 18.941\n"
                                          " c 45.484 18.941 l 45.492 18.977 45.5 19.07 45.508 19.215 c 45.516 19.359\n"
                                          " 45.523 19.531 45.531 19.727 c 45.539 19.914 45.543 20.121 45.543 20.34 \n"
                                          "c 45.551 20.551 45.555 20.746 45.555 20.93 c 45.555 25.465 l 42.484 25.465\n"
                                          " l 42.484 11.719 l 42.484 11.113 42.473 10.574 42.453 10.102 c 42.438 9.629\n"
                                          " 42.422 9.262 42.398 8.996 c 45.383 8.996 l 45.398 9.047 45.41 9.145 45.426\n"
                                          " 9.293 c 45.449 9.438 45.461 9.605 45.469 9.797 c 45.484 9.984 45.496 10.184\n"
                                          " 45.5 10.387 c 45.508 10.59 45.512 10.77 45.512 10.922 c 45.555 10.922 l\n"
                                          " 45.926 10.141 46.438 9.586 47.086 9.25 c 47.734 8.914 48.484 8.746 49.336\n"
                                          " 8.746 c 50.086 8.746 50.734 8.898 51.281 9.203 c 51.828 9.508 52.277 9.93\n"
                                          " 52.625 10.469 c 52.98 11.008 53.246 11.652 53.41 12.402 c 53.586 13.145\n"
                                          " 53.672 13.961 53.672 14.852 c 50.469 14.852 m 50.469 13.504 50.266 12.508\n"
                                          " 49.855 11.859 c 49.449 11.203 48.84 10.875 48.031 10.875 c 47.727 10.875\n"
                                          " 47.418 10.941 47.113 11.07 c 46.816 11.195 46.547 11.414 46.305 11.727 \n"
                                          "c 46.07 12.031 45.879 12.449 45.727 12.973 c 45.582 13.488 45.508 14.145\n"
                                          " 45.508 14.941 c 45.508 15.715 45.582 16.359 45.727 16.875 c 45.871 17.387\n"
                                          " 46.062 17.793 46.293 18.098 c 46.535 18.402 46.805 18.621 47.102 18.754\n"
                                          " c 47.398 18.879 47.703 18.941 48.008 18.941 c 48.402 18.941 48.75 18.863\n"
                                          " 49.059 18.711 c 49.363 18.551 49.621 18.309 49.824 17.98 c 50.035 17.645\n"
                                          " 50.195 17.219 50.305 16.703 c 50.414 16.188 50.469 15.57 50.469 14.855 \n"
                                          "c 56.16 20.82 m 56.16 11.773 l 56.16 11.52 56.156 11.25 56.148 10.965 c \n"
                                          "56.148 10.68 56.141 10.41 56.125 10.156 c 56.117 9.895 56.109 9.66 56.102\n"
                                          " 9.457 c 56.094 9.246 56.082 9.094 56.07 9 c 59 9 l 59.016 9.086 59.027 \n"
                                          "9.242 59.043 9.457 c 59.059 9.668 59.07 9.902 59.086 10.156 c 59.102 10.41\n"
                                          " 59.113 10.668 59.117 10.922 c 59.133 11.168 59.141 11.371 59.141 11.523\n"
                                          " c 59.184 11.523 l 59.336 11.094 59.488 10.711 59.641 10.375 c 59.793 10.031\n"
                                          " 59.973 9.746 60.176 9.512 c 60.387 9.27 60.637 9.09 60.93 8.965 c 61.223\n"
                                          " 8.832 61.582 8.77 62.012 8.77 c 62.195 8.77 62.371 8.789 62.547 8.824 c\n"
                                          " 62.73 8.852 62.867 8.891 62.961 8.934 c 62.961 11.5 l 62.766 11.457 62.562\n"
                                          " 11.418 62.359 11.391 c 62.164 11.355 61.926 11.336 61.648 11.336 c 60.883\n"
                                          " 11.336 60.285 11.645 59.855 12.266 c 59.434 12.887 59.223 13.801 59.223\n"
                                          " 15.02 c 59.223 20.82 l 56.152 20.82 l 76.082 14.898 m 76.082 15.801 75.953\n"
                                          " 16.629 75.699 17.379 c 75.453 18.129 75.078 18.777 74.574 19.324 c 74.07\n"
                                          " 19.863 73.445 20.285 72.695 20.59 c 71.945 20.887 71.07 21.039 70.074 21.039\n"
                                          " c 69.113 21.039 68.266 20.891 67.527 20.59 c 66.793 20.293 66.172 19.871\n"
                                          " 65.668 19.332 c 65.172 18.793 64.797 18.148 64.543 17.398 c 64.289 16.641\n"
                                          " 64.16 15.809 64.16 14.895 c 64.16 14.012 64.281 13.199 64.52 12.457 c 64.766\n"
                                          " 11.707 65.141 11.059 65.633 10.512 c 66.129 9.965 66.75 9.539 67.5 9.234\n"
                                          " c 68.25 8.93 69.129 8.777 70.133 8.777 c 71.195 8.777 72.102 8.93 72.855\n"
                                          " 9.234 c 73.605 9.539 74.219 9.965 74.691 10.512 c 75.172 11.051 75.523 \n"
                                          "11.695 75.742 12.445 c 75.969 13.188 76.082 14.004 76.082 14.895 c 72.871\n"
                                          " 14.895 m 72.871 13.488 72.641 12.469 72.184 11.836 c 71.727 11.203 71.059\n"
                                          " 10.887 70.184 10.887 c 69.281 10.887 68.59 11.207 68.109 11.848 c 67.629\n"
                                          " 12.488 67.387 13.504 67.387 14.895 c 67.387 15.602 67.449 16.211 67.574\n"
                                          " 16.719 c 67.707 17.23 67.887 17.648 68.121 17.977 c 68.355 18.305 68.633\n"
                                          " 18.547 68.961 18.707 c 69.289 18.859 69.648 18.938 70.043 18.938 c 70.496\n"
                                          " 18.938 70.895 18.859 71.246 18.707 c 71.602 18.547 71.902 18.305 72.141\n"
                                          " 17.977 c 72.383 17.648 72.562 17.23 72.688 16.719 c 72.812 16.207 72.875\n"
                                          " 15.602 72.875 14.895 c 84.961 20.816 m 81.289 20.816 l 77.059 8.992 l 80.305\n"
                                          " 8.992 l 82.371 15.602 l 82.438 15.82 82.504 16.055 82.578 16.301 c 82.652\n"
                                          " 16.543 82.719 16.781 82.785 17.023 c 82.852 17.266 82.914 17.496 82.973\n"
                                          " 17.723 c 83.039 17.949 83.098 18.152 83.148 18.336 c 83.191 18.16 83.246\n"
                                          " 17.965 83.312 17.746 c 83.379 17.52 83.445 17.289 83.508 17.047 c 83.582\n"
                                          " 16.805 83.652 16.566 83.727 16.324 c 83.809 16.082 83.883 15.855 83.957\n"
                                          " 15.637 c 86.109 8.992 l 89.32 8.992 l 84.961 20.816 l 95.832 21.035 m 94.98\n"
                                          " 21.035 94.211 20.91 93.527 20.664 c 92.852 20.41 92.27 20.027 91.789 19.516\n"
                                          " c 91.309 19 90.941 18.355 90.684 17.582 c 90.43 16.801 90.301 15.891 90.301\n"
                                          " 14.852 c 90.301 13.723 90.449 12.773 90.75 12 c 91.055 11.227 91.465 10.605\n"
                                          " 91.973 10.133 c 92.488 9.652 93.082 9.305 93.754 9.094 c 94.426 8.883 95.129\n"
                                          " 8.777 95.875 8.777 c 96.809 8.777 97.602 8.941 98.258 9.27 c 98.922 9.59\n"
                                          " 99.465 10.043 99.887 10.625 c 100.309 11.207 100.617 11.906 100.816 12.723\n"
                                          " c 101.012 13.531 101.113 14.426 101.113 15.41 c 101.113 15.496 l 93.539\n"
                                          " 15.508 l 93.539 16.004 93.582 16.465 93.672 16.895 c 93.758 17.316 93.902\n"
                                          " 17.684 94.098 18 c 94.293 18.305 94.551 18.551 94.863 18.73 c 95.176 18.906\n"
                                          " 95.555 18.992 96 18.992 c 96.539 18.992 96.98 18.879 97.32 18.652 c 97.664\n"
                                          " 18.418 97.906 18.062 98.051 17.582 c 100.945 17.832 l 100.812 18.168 100.629\n"
                                          " 18.523 100.387 18.902 c 100.152 19.281 99.84 19.629 99.445 19.953 c 99.051\n"
                                          " 20.266 98.559 20.527 97.969 20.738 c 97.387 20.941 96.676 21.043 95.84 \n"
                                          "21.043 c 95.84 10.719 m 95.527 10.719 95.23 10.773 94.953 10.883 c 94.684\n"
                                          " 10.984 94.445 11.152 94.242 11.387 c 94.047 11.613 93.887 11.906 93.762\n"
                                          " 12.273 c 93.637 12.637 93.57 13.074 93.555 13.586 c 98.145 13.586 l 98.086\n"
                                          " 12.633 97.855 11.918 97.457 11.445 c 97.055 10.965 96.516 10.723 95.84 \n"
                                          "10.723 c 111.117 20.828 m 111.102 20.77 111.086 20.668 111.062 20.523 c \n"
                                          "111.047 20.371 111.031 20.199 111.008 20.012 c 110.992 19.824 110.98 19.629\n"
                                          " 110.965 19.434 c 110.957 19.238 110.953 19.062 110.953 18.91 c 110.91 18.91\n"
                                          " l 110.555 19.676 110.055 20.227 109.414 20.559 c 108.781 20.887 108.02 \n"
                                          "21.051 107.129 21.051 c 106.387 21.051 105.742 20.898 105.195 20.594 c 104.656\n"
                                          " 20.289 104.207 19.863 103.852 19.316 c 103.5 18.77 103.238 18.125 103.066\n"
                                          " 17.383 c 102.898 16.633 102.816 15.816 102.816 14.934 c 102.816 14.039 \n"
                                          "102.902 13.215 103.078 12.465 c 103.262 11.715 103.535 11.07 103.91 10.531\n"
                                          " c 104.281 9.984 104.75 9.559 105.32 9.254 c 105.895 8.949 106.578 8.797\n"
                                          " 107.363 8.797 c 107.75 8.797 108.121 8.836 108.477 8.918 c 108.832 9 109.164\n"
                                          " 9.121 109.473 9.289 c 109.777 9.457 110.055 9.672 110.305 9.934 c 110.551\n"
                                          " 10.195 110.762 10.512 110.938 10.883 c 110.961 10.883 l 110.961 10.809 \n"
                                          "110.957 10.703 110.949 10.566 c 110.949 10.422 110.949 10.258 110.949 10.074\n"
                                          " c 110.949 9.891 110.945 9.703 110.938 9.508 c 110.938 9.312 110.938 9.121\n"
                                          " 110.938 8.941 c 110.938 4.625 l 114.008 4.625 l 114.008 18.262 l 114.008\n"
                                          " 18.836 114.02 19.352 114.039 19.801 c 114.062 20.246 114.078 20.59 114.094\n"
                                          " 20.84 c 111.121 20.84 l 110.98 14.863 m 110.98 14.082 110.906 13.438 110.762\n"
                                          " 12.93 c 110.617 12.414 110.422 12.004 110.184 11.707 c 109.949 11.402 109.68\n"
                                          " 11.191 109.375 11.074 c 109.078 10.949 108.77 10.887 108.457 10.887 c 108.062\n"
                                          " 10.887 107.715 10.965 107.406 11.117 c 107.109 11.27 106.852 11.512 106.641\n"
                                          " 11.84 c 106.438 12.168 106.281 12.586 106.172 13.098 c 106.07 13.609 106.02\n"
                                          " 14.223 106.02 14.945 c 106.02 17.625 106.824 18.965 108.434 18.965 c 108.738\n"
                                          " 18.965 109.047 18.898 109.352 18.77 c 109.656 18.637 109.93 18.418 110.172\n"
                                          " 18.102 c 110.414 17.789 110.605 17.371 110.75 16.844 c 110.902 16.312 110.98\n"
                                          " 15.652 110.98 14.867 c S Q\n"
                                          "Q q\n"
                                          "0 0 127 26.484 re W n\n"
                                          "0 0.298039 0.431373 rg /a1 gs\n"
                                          "3.398 0.398 m 1.77 0.398 0.398 1.77 0.398 3.398 c 0.398 23.086 l 0.398 \n"
                                          "24.715 1.77 26.086 3.398 26.086 c 123.609 26.086 l 125.238 26.086 126.609\n"
                                          " 24.715 126.609 23.086 c 126.609 3.398 l 126.609 1.77 125.238 0.398 123.609\n"
                                          " 0.398 c h\n"
                                          "3.867 3.844 m 123.164 3.844 l 123.164 22.617 l 3.867 22.617 l h\n"
                                          "3.867 3.844 m f\n"
                                          "Q q\n"
                                          "1 1 1 RG /a1 gs\n"
                                          "0.797243 w\n"
                                          "1 J\n"
                                          "1 j\n"
                                          "[] 0.0 d\n"
                                          "4 M q 1 0 0 1 0 0 cm\n"
                                          "3.398 0.398 m 1.77 0.398 0.398 1.77 0.398 3.398 c 0.398 23.086 l 0.398 \n"
                                          "24.715 1.77 26.086 3.398 26.086 c 123.609 26.086 l 125.238 26.086 126.609\n"
                                          " 24.715 126.609 23.086 c 126.609 3.398 l 126.609 1.77 125.238 0.398 123.609\n"
                                          " 0.398 c h\n"
                                          "3.867 3.844 m 123.164 3.844 l 123.164 22.617 l 3.867 22.617 l h\n"
                                          "3.867 3.844 m S Q\n"
                                          "Q\n";

static Dict *getApprovedStampExtGStateDict(PDFDoc *doc)
{
    Dict *a0Dict = new Dict(doc->getXRef());
    a0Dict->add("CA", Object(0.588235));
    a0Dict->add("ca", Object(0.588235));

    Dict *a1Dict = new Dict(doc->getXRef());
    a1Dict->add("CA", Object(1));
    a1Dict->add("ca", Object(1));

    Dict *extGStateDict = new Dict(doc->getXRef());
    extGStateDict->add("a0", Object(a0Dict));
    extGStateDict->add("a1", Object(a1Dict));

    return extGStateDict;
}

#endif