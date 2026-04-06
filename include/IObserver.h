#pragma once

// ============================================================
// Паттерн Observer — абстрактный интерфейс наблюдателя
// ============================================================
class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onStatusUpdate() = 0;
};
