/* libcomps - C alternative to yum.comps library
 * Copyright (C) 2013 Jindrich Luza
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to  Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA
 */

/*! \file comps.h
 * \brief A Documented file.
 * Details.
 * */

#ifndef COMPS_H
#define COMPS_H

#include <stdio.h>
#include <stdarg.h>

#include "comps_doc_types.h"
#include "comps_getset.h"
#include "comps_dict.h"
#include "comps_list.h"
#include "comps_set.h"
#include "comps_logger.h"
#include "comps_hslist.h"
#include "comps_utils.h"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/tree.h>

COMPS_Prop * __comps_doccat_get_prop(void *cat, const char *key);
COMPS_Prop * __comps_docenv_get_prop(void *env, const char *key);
COMPS_Prop * __comps_docgroup_get_prop(void *group, const char *key);


void __comps_doc_char_setter(void **attr, char *val, char copy);
char __comps_doc_add_lobject(COMPS_Doc *doc, char *parent, void *obj,
                                   void(*obj_destructor)(void*));
COMPS_List* __comps_get_lobject(COMPS_Doc *doc, const char *obj);
void __comps_set_lobject(COMPS_Doc *doc, const char *obj, COMPS_List *lobj);
char __comps_doc_remove_lobject(COMPS_Doc *doc, char *parent, void *obj);

COMPS_Dict* __comps_get_dobject(COMPS_Doc *doc, const char *obj);
void __comps_set_dobject(COMPS_Doc *doc, const char *obj, COMPS_Dict *dobj);
char __comps_doc_add_dobject(COMPS_Doc *doc, char *parent, char *key,
                              char *val);

COMPS_MDict* __comps_get_mdobject(COMPS_Doc *doc, const char *obj);
void __comps_set_mdobject(COMPS_Doc *doc, const char *obj, COMPS_MDict *mdobj);
char __comps_doc_add_mdobject(COMPS_Doc *doc, char *parent, char *key,
                              char *val);

void __comps_doc_add_lang_prop(COMPS_Dict *dict, char *lang, char *prop,
                               char copy);
void __comps_doc_add_prop(COMPS_Dict *dict, char *key, COMPS_Prop * prop);
char __comps_doc_write_simple_prop(xmlTextWriterPtr writer, char *prop,
                                   char *val);
void __comps_doc_write_lang_prop(xmlTextWriterPtr writer, COMPS_Dict *lang_dict,
                                 char *prop);


void comps2xml_f(COMPS_Doc * doc, char *filename, char stdoutredirect);
char *comps2xml_str(COMPS_Doc *doc);

COMPS_Doc *comps_doc_create(const char *encoding);
void comps_doc_init(COMPS_Doc *doc);
void comps_doc_clear(COMPS_Doc *doc);
void comps_doc_destroy(COMPS_Doc **doc);
void comps_doc_destroy_v(void *doc);

char comps_doc_add_group(COMPS_Doc *doc, COMPS_DocGroup *group);
char comps_doc_add_category(COMPS_Doc *doc, COMPS_DocCategory *category);
char comps_doc_add_env(COMPS_Doc *doc, COMPS_DocEnv *env);

COMPS_List* comps_doc_get_groups(COMPS_Doc *doc, char *id, char *name,
                                     char *desc, char *lang);
COMPS_List* comps_doc_get_categories(COMPS_Doc *doc, char *id, char *name,
                                     char *desc, char *lang);
COMPS_List* comps_doc_get_envs(COMPS_Doc *doc, char *id, char *name,
                                                 char *desc, char *lang);

char comps_doc_del_group(COMPS_Doc *doc, COMPS_DocGroup *group);
char comps_doc_del_category(COMPS_Doc *doc, COMPS_DocCategory *category);
char comps_doc_del_env(COMPS_Doc * doc, COMPS_DocEnv *env);

void comps_doc_xml(COMPS_Doc * doc, xmlTextWriterPtr writer);
COMPS_Doc* comps_doc_union(COMPS_Doc *c1, COMPS_Doc *c2);
COMPS_Doc* comps_doc_intersect(COMPS_Doc *c1, COMPS_Doc *c2);
char comps_doc_cmp(COMPS_Doc *c1, COMPS_Doc *c2);

COMPS_DocGroup* comps_docgroup_create();
void comps_docgroup_destroy(void *group);
void comps_docgroup_set_id(COMPS_DocGroup *group, char *id, char copy);
void comps_docgroup_set_name(COMPS_DocGroup *group, char *name,
                             char copy);
void comps_docgroup_add_lang_name(COMPS_DocGroup *group, char *lang, char *name,
                                  char copy);
void comps_docgroup_set_desc(COMPS_DocGroup *group, char *desc,
                             char copy);
void comps_docgroup_add_lang_desc(COMPS_DocGroup *group, char *lang, char *desc,
                                  char copy);
void comps_docgroup_set_default(COMPS_DocGroup *group, unsigned def);
void comps_docgroup_set_uservisible(COMPS_DocGroup *group,
                                    unsigned uservisible);
void comps_docgroup_set_displayorder(COMPS_DocGroup *group,
                                     unsigned display_order);
void comps_docgroup_set_langonly(COMPS_DocGroup *group, char *langonly,
                                                        char copy);
void comps_docgroup_add_package(COMPS_DocGroup *group,
                                COMPS_DocGroupPackage *package);
COMPS_List* comps_docgroup_get_packages(COMPS_DocGroup *group, char *name,
                                       COMPS_PackageType type);
void comps_docgroup_xml(COMPS_DocGroup *group, xmlTextWriterPtr writer,
                        COMPS_Logger *log);
COMPS_DocGroup* comps_docgroup_union(COMPS_DocGroup *g1, COMPS_DocGroup *g2);
COMPS_DocGroup* comps_docgroup_intersect(COMPS_DocGroup *g1,
                                         COMPS_DocGroup *g2);
COMPS_DocGroup* comps_docgroup_clone(COMPS_DocGroup *g);
char __comps_docgroup_idcmp(void* g1, void *g2);
char comps_docgroup_cmp(COMPS_DocGroup *g1, COMPS_DocGroup *g2);
char comps_docgroup_cmp_v(void *g1, void *g2);


COMPS_DocCategory* comps_doccategory_create();
void comps_doccategory_destroy(void *category);
void comps_doccategory_set_id(COMPS_DocCategory *category, char *id, char copy);
void comps_doccategory_set_name(COMPS_DocCategory *category, char *name,
                                char copy);
void comps_doccategory_add_lang_name(COMPS_DocCategory *category, char *lang,
                                     char *name, char copy);
void comps_doccategory_set_desc(COMPS_DocCategory *category, char *desc,
                                char copy);
void comps_doccategory_add_lang_desc(COMPS_DocCategory *category, char *lang,
                                     char *desc, char copy);
void comps_doccategory_set_displayorder(COMPS_DocCategory *category,
                                        int display_order);
void comps_doccategory_add_groupid(COMPS_DocCategory *category,
                                   COMPS_DocGroupId *groupid);
void comps_doccategory_xml(COMPS_DocCategory *cat, xmlTextWriterPtr writer,
                           COMPS_Logger *log);
COMPS_DocCategory* comps_doccategory_union(COMPS_DocCategory *c1,
                                           COMPS_DocCategory *c2);
COMPS_DocCategory* comps_doccategory_intersect(COMPS_DocCategory *c1,
                                         COMPS_DocCategory *c2);
COMPS_DocCategory* comps_doccategory_clone(COMPS_DocCategory *c);
char __comps_doccategory_idcmp(void* cat1, void *cat2);
char comps_doccategory_cmp(COMPS_DocCategory *c1, COMPS_DocCategory *c2);
char comps_doccategory_cmp_v(void *c1, void *c2);

COMPS_DocEnv *comps_docenv_create();
void comps_docenv_destroy(void *env);
void comps_docenv_set_id(COMPS_DocEnv * env, char *id, char copy);
void comps_docenv_set_name(COMPS_DocEnv * env, char *name, char copy);
void comps_docenv_add_lang_name(COMPS_DocEnv *env, char *lang,
                                char *name, char copy);
void comps_docenv_set_desc(COMPS_DocEnv * env, char *desc, char copy);
void comps_docenv_add_lang_desc(COMPS_DocEnv *env, char *lang,
                                char *desc, char copy);
void comps_docenv_set_displayorder(COMPS_DocEnv *env, int display_order);
void comps_docenv_add_optionid(COMPS_DocEnv *env,
                               COMPS_DocGroupId *optionid);
void comps_docenv_add_groupid(COMPS_DocEnv *env,
                              COMPS_DocGroupId *groupid);
void comps_docenv_xml(COMPS_DocEnv *env, xmlTextWriterPtr writer,
                      COMPS_Logger *log);
COMPS_DocEnv* comps_docenv_union(COMPS_DocEnv *e1, COMPS_DocEnv *e2);
COMPS_DocEnv* comps_docenv_intersect(COMPS_DocEnv *e1, COMPS_DocEnv *e2);
COMPS_DocEnv* comps_docenv_clone(COMPS_DocEnv *g);
char __comps_docenv_idcmp(void* e1, void *e2);
char comps_docenv_cmp(COMPS_DocEnv *e1, COMPS_DocEnv *e2);
char comps_docenv_cmp_v(void *e1, void *e2);

COMPS_DocGroupPackage* comps_docpackage_create();
COMPS_DocGroupPackage* comps_docpackage_clone(COMPS_DocGroupPackage * pkg);
void comps_docpackage_set_name(COMPS_DocGroupPackage *package, char *name,
                               char copy);
void comps_docpackage_set_type(COMPS_DocGroupPackage *package,
                               COMPS_PackageType type);
void comps_docpackage_set_requires(COMPS_DocGroupPackage *package,
                                   char *requires, char copy);

void comps_docpackage_destroy(void *pkg);
void comps_docpackage_xml(COMPS_DocGroupPackage *pkg, xmlTextWriterPtr writer,
                          COMPS_Logger *log);
char comps_docpackage_cmp(void* pkg1, void *pkg2);
const char* comps_docpackage_type_str(COMPS_PackageType type);

COMPS_DocGroupId* comps_docgroupid_create();
COMPS_DocGroupId* comps_docgroupid_clone(COMPS_DocGroupId * groupid);
void comps_docgroupid_set_name(COMPS_DocGroupId *groupid, char *name, char copy);
void comps_docgroupid_set_default(COMPS_DocGroupId *groupid, char def);
char comps_docgroupid_cmp(void* gid1, void *gid2);
void comps_docgroupid_destroy(void *groupid);

#endif
