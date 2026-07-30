### File Overview
- Design Document - Description of Approach (this file)
- [Milestone 1 - Structure, Config Parsing and Printing](Milestone_1.md)
- [Milestone 2 - Commands, Game Logic and Special Powers](Milestone_2.md)
- [Piece Overview (file listing all pieces and their special powers)](Pieces.md)
- [Items Overview (file listing all items, their powers and usage)](Items.md)
- [Error Overview (file listing all errors that must be handled)](Errors.md)

# Design Document

The purpose of this design document is to help you understand the assignment description and the game before implementing your solution. You should decide on the basic structure and design of your program, and then discuss your ideas with other teams during a discussion round. If you later realize that changes to the structure are necessary, that is of course possible.

The aim of the discussion round is to help you establish a well-structured design early in the implementation process. Participation in the discussion round is mandatory, and it is important that every team member actively contributes.

> **Note:** Submitting the design document is not worth any points. However, handing in a document that demonstrates effort is mandatory in order to receive a positive grade for the practicals (KU).

## Content of the Design Document

The design document should include information about the following:
- The classes you will use in your implementation
- A short description (1-2 sentences or keywords) of the purpose of each class
- A list of the most important methods and member variables of each class (full function headers and data types are not required)
- The relationships between classes (e.g., inheritance, composition)

You should also visualize your design using a **UML diagram**. Simple hand-drawn or digital diagrams are both acceptable. The visualization should help convey the structure of your program and does not need to be overly detailed.

## Questions to ask yourself

Below are a few questions to consider and discuss with your team when deciding on the design. This list is not exhaustive, and you are encouraged to add your own questions.

- What is the overall class structure? How do the objects interact with each other?
- How are board, players, and pieces represented?
- How are turns and rounds structured in your program?
- How are special abilities, mana, and items handled?
- How do you model movement rules of pieces?
- How are items represented, stored, and used during the game?
- How are user commands parsed and executed?
- ...