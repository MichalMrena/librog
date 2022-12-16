#ifndef ROG_DETAILS_VISITORS_HPP
#define ROG_DETAILS_VISITORS_HPP

namespace rog
{
    class LeafTest;
    class CompositeTest;

    /**
     *  \brief Test visitor interface.
     */
    struct IVisitor
    {
        virtual ~IVisitor () = default;
        virtual auto visit (LeafTest&) -> void  = 0;
        virtual auto visit (CompositeTest&) -> void  = 0;
    };

    /**
     *  \brief Visitable interface implemented by tests.
     */
    struct IVisitable
    {
        virtual ~IVisitable () = default;
        virtual auto accept (IVisitor&) -> void = 0;
    };
}

#endif