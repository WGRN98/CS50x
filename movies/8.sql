--SELECT name FROM people
--JOIN stars ON people.id = stars.person_id
--JOIN movies ON stars.movie_id = movies.id
--WHERE title = 'Toy Story';

-- Select names
SELECT name
FROM people
WHERE id IN
(
    -- Select person IDs
    SELECT person_id
    FROM stars
    WHERE movie_id = (

        -- Select Toy Story's ID
        SELECT id
        FROM movies
        WHERE title = 'Toy Story'
    )
);
