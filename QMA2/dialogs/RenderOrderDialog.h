/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2010-2012  hkrn                                    */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAI project team nor the names of     */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

#ifndef RENDERORDERDIALOG_H
#define RENDERORDERDIALOG_H

#include <QtCore/QString>
#include <QtCore/QUuid>
#include <QtGui/QDialog>

class QAbstractButton;
class QDialogButtonBox;
class QListWidget;
class SceneLoader;

class RenderOrderDialog : public QDialog
{
    Q_OBJECT

public:
    typedef QPair<QString, QUuid> NameUUID;

    explicit RenderOrderDialog(SceneLoader *loader, QWidget *parent = 0);
    ~RenderOrderDialog();

signals:
    void renderOrderListDidSet(const QList<QUuid> &uuid);

private slots:
    void retranslate();
    void emitSignal();
    void setOrderUp();
    void setOrderDown();
    void resetOrder();
    void handleButton(QAbstractButton *button);

private:
    void buildOriginFromRenderOrder(const SceneLoader *loader);
    void setRenderOrder(const QList<NameUUID> &pairs);

    QListWidget *m_listWidget;
    QDialogButtonBox *m_dialogButtonBox;
    QPushButton *m_upButton;
    QPushButton *m_downBotton;
    QPushButton *m_resetButton;
    QList<NameUUID> m_origin;
};

#endif // RENDERORDERDIALOG_H
