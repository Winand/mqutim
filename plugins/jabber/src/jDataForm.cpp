/*
jDataForm.cpp

Copyright (c) 2008 by Kostin Dmitrij <kostindima@gmail.com>

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
*/


#include "jDataForm.h"

jDataForm::jDataForm(gloox::DataForm * form, QWidget *parent) : QWidget(parent)
{
    forma = new DataForm(*form);
    forma->setType(TypeSubmit);

    gridLayout = new QGridLayout(this);

    this->setLayout(gridLayout);
    DataFormFieldContainer::FieldList lst = form->fields();
    DataFormFieldContainer::FieldList::iterator it;
    for ( it=lst.begin() ; it != lst.end(); it++ ){
    	//if ( (*it)->type() == DataFormField::TypeHidden || (*it)->type() == DataFormField::TypeNone || (*it)->type() == DataFormField::TypeInvalid) continue;

	laouts.append(new QHBoxLayout());
        labels.append(new QLabel());
	laouts.last()->addWidget(labels.last(), 0);
	laouts.last()->setObjectName("laout_"+QString::number(laouts.count()));
	if ((*it)->type()==DataFormField::TypeFixed){
	    labels.last()->setText(jProtocol::fromStd((*it)->value()));
	} else labels.last()->setText(jProtocol::fromStd((*it)->label()));
	    qDebug() << jProtocol::fromStd((*it)->label());
	if ( (*it)->type()==DataFormField::TypeTextPrivate || (*it)->type()==DataFormField::TypeTextSingle ){
            line_edits.append(new QLineEdit());
	    line_edits.last()->setObjectName(jProtocol::fromStd((*it)->name()));
	    if ( (*it)->type()==DataFormField::TypeTextPrivate )
			line_edits.last()->setEchoMode(QLineEdit::Password);
	    line_edits.last()->setText(jProtocol::fromStd((*it)->value()));
	    laouts.last()->addWidget(line_edits.last(), 1);
	    gridLayout->addLayout(laouts.last(), laouts.count()+1, 0);
	}
	else if ( (*it)->type()==DataFormField::TypeTextMulti ){
            text_edits.append(new QTextEdit());
	    text_edits.last()->setObjectName(jProtocol::fromStd((*it)->name()));
            text_edits.last()->setText(jProtocol::fromStd((*it)->value()));
            gridLayout->addLayout(laouts.last(), laouts.count()+1, 0);
            laouts.append(new QHBoxLayout());
            laouts.last()->setObjectName("laout_"+QString::number(laouts.count()));
	    laouts.last()->addWidget(text_edits.last(), 1);
	    gridLayout->addLayout(laouts.last(), laouts.count()+1, 0);
	}
	else if ( (*it)->type()==DataFormField::TypeBoolean ){
            chk_boxes.append(new QCheckBox());
	    chk_boxes.last()->setObjectName(jProtocol::fromStd((*it)->name()));
	    laouts.last()->addWidget(chk_boxes.last(), 1);
	    gridLayout->addLayout(laouts.last(), laouts.count()+1, 0);
            chk_boxes.last()->setChecked((*it)->value()=="1");
	}
	else if ( (*it)->type()==DataFormField::TypeListSingle ){
            combo_boxes.append(new QComboBox());
	    combo_boxes.last()->setObjectName(jProtocol::fromStd((*it)->name()));
	    combo_boxes.last()->setInsertPolicy(QComboBox::NoInsert);
            laouts.last()->addWidget(combo_boxes.last(), 1);
            StringList values = (*it)->values();
	}
	gridLayout->addLayout(laouts.last(), laouts.count()+1, 0);
    }
}

jDataForm::~jDataForm()
{
    //TODO: Need another ~ metod
    /*qDeleteAll(laouts);
    qDeleteAll(labels);
    qDeleteAll(line_edits);
    qDeleteAll(chk_boxes);
    qDeleteAll(combo_boxes);
    qDeleteAll(text_edits);*/
}

DataForm *jDataForm::getDataForm()
{
	for (int i = 0; i < line_edits.size(); ++i) {
		forma->field(jProtocol::toStd(line_edits[i]->objectName()))->setValue(jProtocol::toStd(line_edits[i]->text()));
	}
	for (int i = 0; i < text_edits.size(); ++i) {
		forma->field(jProtocol::toStd(text_edits[i]->objectName()))->setValue(jProtocol::toStd(text_edits[i]->toPlainText()));
	}
	for (int i = 0; i < chk_boxes.size(); ++i) {
                forma->field(chk_boxes[i]->objectName().toAscii().data())->setValue(chk_boxes[i]->isChecked()?"1":"0");
	}
	for (int i = 0; i < combo_boxes.size(); ++i) {
		forma->field(jProtocol::toStd(combo_boxes[i]->objectName()))->setValue(jProtocol::toStd(combo_boxes[i]->currentText()));
	}
	return forma;
}
