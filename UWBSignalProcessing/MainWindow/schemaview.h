#ifndef SCHEMAVIEW_H
#define SCHEMAVIEW_H

#include <QGraphicsView>
#include <QList>
#include "schemticsscene.h"

class SchemaView
{

public:
    SchemaView(const QString &name);
    ~SchemaView();
    void CreateContextMenu(QList<QAction*> itemsMenu);
    void ZoomOutScheme();
    void ZoomOriginalScheme();
    void ZoomInScheme();

    void OpenScheme(const QString& path);
    void SaveScheme(const QString& path);
    void DelBlock();
    void CutBlock();
    void CopyBlock();
    void PasteBlock();
    void PropertiesBlock();
    void SelecteAll();

    void VerticalAligning();
    void HorizontalAligning();

    void Start();
    void Pause();
    void Stop();

    void ShowTimer();

    bool RenameScheme(const QString& nameSchema);

    // состояние сцены
    bool IsStart() const;
    bool HasSelection() const;
    bool IsSelecteBlock() const;
    bool IsSelecteBlocks() const;
    bool IsPaste() const;
    bool IsEmpty() const;

    QGraphicsView *GetViewOfSchema() const;
    const SchemticsController* GetSchemaController() const;

    void SetOperationManager(AbstractOperationManager* manager);


private:
    void CreateView(const QString &name);
    // Объект представления для отображения схемы из блоков
    QGraphicsView* m_viewOfSchema;
    SchemticsController* m_schemaController;
};

#endif // SCHEMAVIEW_H
