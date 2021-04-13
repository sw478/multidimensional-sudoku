package org.sat4j.pb.multiobjective;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.NoSuchElementException;

import org.junit.Before;
import org.junit.Test;
import org.sat4j.pb.multiobjective.MinSumOWAOptimizer.PermutationComputer;

public class PermutationIteratorTest {

    private PermutationComputer iterator;

    @Before
    public void setup() {
        iterator = new MinSumOWAOptimizer.PermutationComputer(3);
    }

    @Test
    public void testExpectedBehavior() {
        for (int i = 0; i < 6; i++) {
            assertTrue(iterator.hasNext());
            assertNotNull(iterator.next());
        }
        assertFalse(iterator.hasNext());
    }

    @Test(expected = NoSuchElementException.class)
    public void testExceptionWhenNoMoreSolutions() {
        for (int i = 0; i < 6; i++) {
            assertTrue(iterator.hasNext());
            assertNotNull(iterator.next());
        }
        assertFalse(iterator.hasNext());
        assertNotNull(iterator.next());
    }

}
